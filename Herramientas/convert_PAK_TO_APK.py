import sys
from pathlib import Path

def convert_pak_to_apk(pak_file: Path, apk_file: Path):
    data = pak_file.read_bytes()
    pak_name = pak_file.stem

    if len(data) < 24:
        print(f"⚠️ {pak_file.name} es demasiado pequeño para ser válido.")
        return

    num_entries = int.from_bytes(data[20:24], 'little')
    index_start = 24
    sprites = []

    # Leer entradas
    for i in range(num_entries):
        off = int.from_bytes(data[index_start + i*8 : index_start + i*8 + 4], 'little')
        size = int.from_bytes(data[index_start + i*8 + 4 : index_start + i*8 + 8], 'little')
        sprites.append((off, size))

    # Crear índice apk-style (77 bytes por entrada, solo escribiremos el offset como los primeros 4 bytes)
    apk_data = bytearray()
    apk_data += data[:20]              # Copiar primeros 20 bytes
    fake_value = 0                     # Esto será ignorado
    apk_data += fake_value.to_bytes(4, 'little')  # Lugar de iTotalImage (se calculará al cargar)

    # Guardar índice (4 bytes por offset, relleno hasta 77 bytes por entrada)
    for off, size in sprites:
        entry = bytearray(77)
        entry[0:4] = off.to_bytes(4, 'little')
        apk_data += entry

    # Añadir los sprites reales
    for off, size in sprites:
        apk_data += data[off:off+size]

    apk_file.write_bytes(apk_data)
    print(f"✅ {pak_file.name} → {apk_file.name} ({apk_file.stat().st_size} bytes)")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Uso: python convert_pak_to_apk.py input.pak output.apk")
    else:
        convert_pak_to_apk(Path(sys.argv[1]), Path(sys.argv[2]))
