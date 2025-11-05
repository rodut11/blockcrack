import ctypes
import numpy as np
import cv2
import sys
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
lib_path = os.path.join(script_dir, '..', 'build', 'get_screencap.so')
project_root = os.path.abspath(os.path.join(script_dir, '..', '..', '..'))
sample_path = os.path.join(project_root, 'assets', 'sample.png')

# --- Load screencap ---
lib = ctypes.CDLL(lib_path)
lib.get_screencap.argtypes = [ctypes.POINTER(ctypes.c_size_t)]
lib.get_screencap.restype = ctypes.POINTER(ctypes.c_ubyte)

size = ctypes.c_size_t()
buffer_ptr = lib.get_screencap(ctypes.byref(size))
img_data = np.ctypeslib.as_array(buffer_ptr, shape=(size.value,))
img = cv2.imdecode(img_data, cv2.IMREAD_COLOR)

# --- Block region ---
x1, y1, x2, y2 = 693, 711, 1183, 855
search_region = img[y1:y2, x1:x2].copy()
gray = cv2.cvtColor(search_region, cv2.COLOR_BGR2GRAY)
_, binary = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY_INV)

# --- Remove small blobs ---
def remove_small_components(mask, min_area=40):
    num_labels, labels, stats, _ = cv2.connectedComponentsWithStats(mask, connectivity=8)
    out = np.zeros_like(mask)
    for i in range(1, num_labels):
        if stats[i, 4] >= min_area:
            out[labels == i] = 255
    return out

cleaned = remove_small_components(binary, min_area=40)

# --- Connected components ---
def get_block_boxes(mask, min_area=64):
    num_labels, labels, stats, _ = cv2.connectedComponentsWithStats(mask, connectivity=8)
    boxes = []
    for i in range(1, num_labels):
        x, y, w, h, area = stats[i]
        if area >= min_area:
            boxes.append((x1 + x, y1 + y, w, h))
    return boxes

# --- Output 2-byte coordinates (center of block) ---
BLOCK_MARKER = b'\xFF'

merged_boxes = sorted(get_block_boxes(cleaned, min_area=64), key=lambda b: b[0])[:3]

for bx, by, bw, bh in merged_boxes:
    cx = int(bx + bw / 2)
    cy = int(by + bh / 2)
    sys.stdout.buffer.write(cx.to_bytes(2, 'big') + cy.to_bytes(2, 'big') + BLOCK_MARKER)
