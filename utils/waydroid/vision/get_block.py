import ctypes
import numpy as np
import cv2
import sys
import os

# --- Connected components ---
def get_block_boxes(mask, min_area=64):
    num_labels, labels, stats, _ = cv2.connectedComponentsWithStats(mask, connectivity=8)
    boxes = []
    for i in range(1, num_labels):
        x, y, w, h, area = stats[i]
        if area >= min_area:
            boxes.append((x, y, w, h))
    return boxes

def detect_cells_grid(block_img, cell_w, cell_h, fill_ratio_thresh=0.55):
    ys, xs = np.where(block_img > 0)
    if len(xs) == 0 or len(ys) == 0:
        return np.zeros((1,1), dtype=np.uint8), []

    x_min, x_max = xs.min(), xs.max()
    y_min, y_max = ys.min(), ys.max()
    cropped = block_img[y_min:y_max+1, x_min:x_max+1]

    h, w = cropped.shape
    n_rows = max(1, h // cell_h)
    n_cols = max(1, w // cell_w)
    cropped = cropped[:n_rows*cell_h, :n_cols*cell_w]

    grid = np.zeros((n_rows, n_cols), dtype=np.uint8)
    for r in range(n_rows):
        for c in range(n_cols):
            cell = cropped[r*cell_h:(r+1)*cell_h, c*cell_w:(c+1)*cell_w]
            if np.sum(cell > 0) / (cell.size + 1e-6) > fill_ratio_thresh:
                grid[r, c] = 1

    return grid, []

# --- Output ---
ROW_SEPARATOR = b"\xFE"
BLOCK_SEPARATOR = b"\xFF"
HEADER_MARKER  = b"\xFD"

merged_boxes = sorted(get_block_boxes(cleaned, min_area=64), key=lambda b: b[0])[:3]

header_bytes = []
grids = []

for block_img in [cleaned[y:y+h, x:x+w] for (x,y,w,h) in merged_boxes]:
    grid, _ = detect_cells_grid(block_img, cell_w, cell_h)
    rows, cols = grid.shape
    header_bytes.extend([rows, cols])
    grids.append(grid)

sys.stdout.buffer.write(bytes(header_bytes) + HEADER_MARKER)

for grid in grids:
    for r in range(grid.shape[0]):
        sys.stdout.buffer.write(grid[r, :].tobytes())
        sys.stdout.buffer.write(ROW_SEPARATOR)
    sys.stdout.buffer.write(BLOCK_SEPARATOR)
