import struct
from pathlib import Path

def extract_bmps_from_pak(pak_path: Path, output_dir: Path):
    data = pak_path.read_bytes()
    pak_name = pak_path.stem
    bmp_count = 0

    if len(data) < 24:
        print(f"⚠️ Archivo muy pequeño: {pak_path.name}")
        return 0

    num_entries = int.from_bytes(data[20:24], 'little')
    index_start = 24
    index_size = 8 * num_entries

    for i in range(num_entries):
        entry_offset = int.from_bytes(data[index_start + i * 8 : index_start + i * 8 + 4], 'little')
        entry_size   = int.from_bytes(data[index_start + i * 8 + 4 : index_start + i * 8 + 8], 'little')
        entry_end    = entry_offset + entry_size

        if entry_offset >= len(data) or entry_end > len(data):
            continue

        chunk = data[entry_offset:entry_end]

        # Buscar la cabecera BMP (firma 'BM' en ASCII)
        bmp_start = chunk.find(b'BM')
        if bmp_start == -1:
            continue

        try:
            bmp_size = int.from_bytes(chunk[bmp_start + 2:bmp_start + 6], 'little')
            bmp_data = chunk[bmp_start : bmp_start + bmp_size]
            if bmp_data[:2] != b'BM':
                continue
            # Guardar BMP
            output_path = output_dir / f"{pak_name}_{bmp_count}.bmp"
            output_path.write_bytes(bmp_data)
            bmp_count += 1
        except Exception as e:
            continue

    return bmp_count

def main():
    pak_files = list(Path(".").glob("*.pak"))
    output_dir = Path("imagenes")
    output_dir.mkdir(exist_ok=True)

    total = 0
    for pak in pak_files:
        count = extract_bmps_from_pak(pak, output_dir)
        print(f"✅ {pak.name}: {count} imagen(es) extraída(s)")
        total += count

    print(f"\n🎉 Total: {total} imagen(es) extraída(s) en '{output_dir}/'")

if __name__ == "__main__":
    main()
