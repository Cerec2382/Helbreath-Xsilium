import sys
from pathlib import Path

def convert_apk_to_pak(apk_path: Path, pak_path: Path):
    data = apk_path.read_bytes()
    fsize = len(data)

    MULT = 77
    idx_start = 24
    offsets = []

    while True:
        pos = idx_start + len(offsets) * MULT
        if pos + 4 > fsize:
            break
        ptr = int.from_bytes(data[pos:pos+4], 'little')
        if ptr <= 0 or ptr >= fsize:
            break
        offsets.append(ptr)

    if not offsets:
        print(f"❌ {apk_path.name}: sin entradas válidas.")
        return

    sizes = []
    for i in range(len(offsets) - 1):
        sizes.append(offsets[i+1] - offsets[i])
    sizes.append(fsize - offsets[-1])

    N = len(offsets)
    pak_data = bytearray()

    header = bytearray(data[:20])
    header += N.to_bytes(4, 'little')
    pak_data += header

    index_table = bytearray()
    blob_base = 24 + 8 * N
    new_offsets = []
    cumulative = blob_base
    for sz in sizes:
        new_offsets.append(cumulative)
        cumulative += sz

    for off, sz, new_off in zip(offsets, sizes, new_offsets):
        index_table += new_off.to_bytes(4, 'little')
        index_table += sz.to_bytes(4, 'little')

    pak_data += index_table

    for off, sz in zip(offsets, sizes):
        pak_data += data[off:off+sz]

    pak_path.parent.mkdir(exist_ok=True)
    pak_path.write_bytes(pak_data)
    print(f"✅ {apk_path.name} → {pak_path.name} ({len(pak_data)} bytes)")

if __name__ == "__main__":
    base_dir = Path(".")
    output_dir = base_dir / "output"
    apk_files = list(base_dir.glob("*.apk"))  # solo en el directorio actual

    if not apk_files:
        print("No se encontraron archivos .apk en el directorio actual.")
        sys.exit(0)

    for apk_file in apk_files:
        pak_file = output_dir / (apk_file.stem + ".pak")
        convert_apk_to_pak(apk_file, pak_file)
