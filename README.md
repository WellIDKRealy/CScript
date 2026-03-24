# CScript

**C jako język skryptowy** – kompilacja w pamięci, zero plików na dysku, pełne wsparcie shebangu.

## Opis

CScript to minimalistyczny wrapper, który pozwala uruchamiać kod źródłowy w języku C dokładnie tak samo jak skrypty Bash, Python czy Ruby.

- Używa **tcc** (Tiny C Compiler) – kompilacja trwa milisekundy.
- Wszystko dzieje się **wyłącznie w pamięci** (`memfd_create` + `fexecve`).
- Żaden plik tymczasowy nie trafia na dysk.
- Pełne wsparcie dla shebangu (`#!/usr/bin/env CScript --`).
- Zmiana kompilatora na **gcc** lub **clang** jest trywialna (jedna linijka).

## Instalacja

```bash
git clone https://github.com/twoje-repo/CScript.git
cd CScript
make
sudo make install