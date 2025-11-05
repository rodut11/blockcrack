import ctypes
import numpy as np
import cv2
import sys
import os

# --- Script setup ---
script_dir = os.path.dirname(os.path.abspath(__file__))
lib_path = os.path.join(script_dir, '..', 'build', 'get_screencap.so')

# --- Load screencap ---
lib = ctypes.CDLL(lib_path)
lib.get_screencap.argtypes = [ctypes.POINTER(ctypes.c_size_t)]
lib.get_screencap.restype = ctypes.POINTER(ctypes.c_ubyte)

size = ctypes.c_size_t()
buffer_ptr = lib.get_screencap(ctypes.byref(size))
img_data = np.ctypeslib.as_array(buffer_ptr, shape=(size.value,))
img = cv2.imdecode(img_data, cv2.IMREAD_COLOR)

# --- Define grid area ---
x1, y1, x2, y2 = 716, 206, 1163, 654
region = img[y1:y2, x1:x2]
gray = cv2.cvtColor(region, cv2.COLOR_BGR2GRAY)
_, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

# --- Remove small junk ---
def remove_small_components(mask, min_area=40):
    num_labels, labels, stats, _ = cv2.connectedComponentsWithStats(mask, connectivity=8)
    cleaned = np.zeros_like(mask)
    for i in range(1, num_labels):
        if stats[i, 4] >= min_area:
            cleaned[labels == i] = 255
    return cleaned

cleaned = remove_small_components(binary, min_area=40)

# --- Divide region into 8x8 grid ---
h, w = cleaned.shape
cell_h, cell_w = h // 8, w // 8
grid = np.zeros((8, 8), dtype=np.uint8)

# --- Detect filled cells (left→right, top→bottom) ---
for r in range(8):
    for c in range(8):
        y_start, y_end = r * cell_h, (r + 1) * cell_h
        x_start, x_end = c * cell_w, (c + 1) * cell_w
        cell = cleaned[y_start:y_end, x_start:x_end]
        fill_ratio = np.sum(cell > 0) / (cell.size + 1e-6)
        grid[r, c] = 1 if fill_ratio > 0.5 else 0

# --- Output bytes in row-major order (L→R, then next row) ---
ROW_SEPARATOR = 0xFE
output_bytes = bytearray()

for r in range(8):
    output_bytes.extend(grid[r, :].tobytes())  # Left to right
    output_bytes.append(ROW_SEPARATOR)         # Row separator

sys.stdout.buffer.write(output_bytes)
