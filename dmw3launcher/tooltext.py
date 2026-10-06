"""English rendering of DiscTool's output.

DiscTool.exe is the recomp project's own disc tool and prints its diagnosis in Spanish. This
launcher is English-only, so anything it says to the player has to be said in English.

The sentence table below is the tool's own message catalogue, mapped one to one; the word table
covers its report labels ("volumen:", "FALTA", "ejecutable:"). Text that is not recognised is
passed through untouched, so a message a future DiscTool adds shows up verbatim instead of being
silently dropped, and the untranslated original is kept in DiscCheck.raw for bug reports.

DiscTool's exit code and the `ES EL DISCO CORRECTO` verdict line are matched in Spanish elsewhere
(disc.verify): that is protocol, not text shown to anyone.
"""
from __future__ import annotations

import re

# Whole messages, in the tool's own wording. Group references (\1) survive translation.
_SENTENCES: tuple[tuple[str, str], ...] = (
    (r"ES EL DISCO CORRECTO", "This is the correct disc."),
    (r"No es el disco correcto\.", "This is not the correct disc."),
    (r"La imagen es demasiado corta: no llega ni al descriptor del disco\. "
     r"Puede que la copia se cortara a medias\.",
     "The image is too short to reach even the disc descriptor, so the copy may have been cut off."),
    (r"Esto no es un disco de datos: no tiene descriptor ISO9660\.",
     "This is not a data disc: it has no ISO9660 descriptor."),
    (r"No es un disco de PlayStation \(pone \"(.*?)\" donde deberia poner PLAYSTATION\)\.",
     r'Not a PlayStation disc: it says "\1" where it should say PLAYSTATION.'),
    (r"Es un disco de PlayStation, pero no es Digimon World 2003: el disco se llama \"(.*?)\" "
     r"y deberia llamarse DMW3\.",
     r'It is a PlayStation disc, but not Digimon World 2003: the disc is named "\1" and should '
     r"be named DMW3."),
    (r"Es Digimon World 3/2003, pero no la edicion europea\. Su disco arranca con (.+?) y este "
     r"port esta hecho sobre (.+?)\. Hace falta la version europea \(SLES-03936\)\.",
     r"It is Digimon World 3/2003, but not the European edition: its disc boots with \1 and "
     r"this port is built from \2. The European version (SLES-03936) is required."),
    (r"El disco es el correcto, pero la imagen esta incompleta: tiene (\d+) sectores y deberia "
     r"tener (\d+)\. Vuelve a copiarla\.",
     r"The disc is the right one, but the image is incomplete: \1 sectors instead of \2. "
     r"Copy it again."),
    (r"No consigo saber a que fichero \.bin apunta ese \.cue\.",
     "Cannot tell which .bin file that .cue points at."),
    (r"No puedo abrir \"(.*?)\"\.", r'Cannot open "\1".'),
    (r"Ese fichero no es una imagen cruda de CD\..*",
     "That file is not a raw CD image: it needs the 2352-bytes-per-sector .bin that comes with "
     "a .cue. A plain .iso will not work, because the movies are stored as raw sectors."),
    (r"\"(.*?)\" no es una imagen cruda de CD\.", r"\1 is not a raw CD image."),
    (r"No he podido escribir la zona de sistema\.", "Could not write the system area."),
    (r"No he podido escribir (.+?)\. Puede que no quede sitio en el disco o que la carpeta este "
     r"protegida\.",
     r"Could not write \1: the disc may be full or the folder may be protected."),
    (r"No he podido escribir (.+?)\.", r"Could not write \1."),
    (r"No he podido crear (.+?)\.", r"Could not create \1."),
    (r"No he podido sacar el ejecutable\.", "Could not extract the executable."),
    (r"Extraccion cancelada\.", "Extraction cancelled."),
    (r"Cancelado\.", "Cancelled."),
    (r"La imagen se acaba antes de tiempo leyendo (.+?)\.", r"The image ends early while reading \1."),
    (r"No encuentro (.+?)\. El portugues se instala SOBRE el juego ya extraido, no en su lugar\.",
     r"Could not find \1. The Portuguese pack installs ON TOP of the already-extracted game, "
     r"not in its place."),
    (r"Esa imagen no trae COUNTRY/ENG: no parece el disco de la traduccion\.",
     "That image has no COUNTRY/ENG, so it does not look like the translation disc."),
    (r"El mapa del juego no trae ningun fichero de COUNTRY/ENG\.",
     "The game's file map contains no COUNTRY/ENG files."),
    (r"El disco no trae (.+?)\.", r"The disc does not contain \1."),
    (r"no encuentro (.+)$", r"not found: \1"),
    (r"SCREEN/TLOGOPAL\.BIN de (.+?) ya tiene las letras del menu",
     r"SCREEN/TLOGOPAL.BIN in \1 already has the menu letters."),
    (r"CONFIG/SLES_039\.36 escrito en (.+)", r"CONFIG/SLES_039.36 written to \1"),
    (r"bg\.png y logo_titulo\.png escritos en (.+)", r"bg.png and logo_titulo.png written to \1"),
    # "SCREEN/STSTATTM.BIN no trae las dos laminas que esperaba (encontre %d)."
    (r"(.+?) no trae las dos laminas que esperaba \(encontre (\d+)\)\.",
     r"\1 does not carry the two sheets it should (found \2)."),
    (r"La lamina de la ficha es mas corta de lo esperado: no llega a la tabla de paletas\.",
     "The stat sheet is shorter than expected: it does not reach the palette table."),
    (r"(.+?) no es la lamina de 8 bits que esperaba\.",
     r"\1 is not the 8-bit sheet it should be."),
    (r"No encuentro el logotipo dentro de la lamina de idioma\.",
     "Cannot find the logo inside the language sheet."),
    (r"no pude abrir (.+)", r"could not open \1"),
    (r"(.+?) no se puede medir o esta vacio", r"\1 cannot be measured or is empty"),
    (r"no pude leer (.+?) entero", r"could not read \1 in full"),
    (r"(.+?): la cabecera apunta fuera del fichero", r"\1: its header points outside the file"),
    (r"(.+?): la primera imagen no se pudo descomprimir", r"\1: its first image would not decompress"),
    (r"(.+?): la primera imagen no es la lamina de 256x256 de 4 bits que se esperaba",
     r"\1: its first image is not the expected 256x256 4-bit sheet"),
    (r"sin memoria al recomprimir la lamina", "out of memory recompressing the sheet"),
    (r"sin memoria al rehacer la lamina", "out of memory rebuilding the sheet"),
    (r"no pude escribir (.+?) entero \(\s*disco lleno\?\)",
     r"could not write \1 in full (disc full?)"),
    (r"no pude escribir (.+)", r"could not write \1"),
    (r"no pude cerrar (.+)", r"could not close \1"),
    (r"no pude apartar (.+)", r"could not set \1 aside"),
    (r"no pude poner (.+?) en su sitio", r"could not put \1 in place"),
    (r"la tabla de rutas tiene un tama[nñ]o imposible \((\d+) bytes\)",
     r"the path table has an impossible size (\1 bytes)"),
    (r"la imagen se acaba dentro de la tabla de rutas", "the image ends inside the path table"),
    (r"nombre de directorio absurdamente largo", "absurdly long directory name"),
    (r"un directorio del disco no se puede leer \(LBA (\d+)\)",
     r"a disc directory cannot be read (LBA \1)"),
    (r"la imagen se acaba dentro de un directorio", "the image ends inside a directory"),
)

# Report labels and one-word values. Applied after the sentences, so only leftovers are hit.
_WORDS: tuple[tuple[str, str], ...] = (
    (r"\bFALTA\b", "MISSING"),
    (r"\bIDENTICO\b", "IDENTICAL"),
    (r"\bDISTINTO\b", "DIFFERENT"),
    (r"\bTAMA[NÑ]O\b", "SIZE"),
    (r"\bvolumen\b", "volume"),
    (r"\bsistema\b", "system"),
    (r"\bejecutable\b", "executable"),
    (r"\bsectores\b", "sectors"),
    (r"\bveredicto\b", "verdict"),
    (r"\bfallo\b", "failure"),
    (r"\bninguno\b", "none"),
    (r"\boculto\b", "hidden"),
    (r"(:\s*)si\b", r"\1yes"),          # the report's yes/no values, only after a colon
    (r"\bportugues\b", "Portuguese"),
    (r"\bletras\b", "letters"),
    (r"\bmio\b", "mine"),
    (r"\bbueno\b", "reference"),
    (r"no esta en el arbol bueno", "not in the reference tree"),
    (r"no esta en el bueno", "not in the reference"),
    (r"ficheros del mapa bueno", "files in the reference map"),
    (r"identicos \(tama[nñ]o\+sha256\)", "identical (size+sha256)"),
    (r"que faltan", "missing"),
    (r"con distinto tama[nñ]o", "with a different size"),
    (r"con distinto sha256", "with a different sha256"),
    (r"bytes cotejados", "bytes compared"),
    # leftovers from the report/usage lines: the word net has to close the entire catalogue,
    # otherwise a fragment such as "(el disco dice 330000)" reaches the player in Spanish
    (r"\bel disco dice\b", "the disc says"),
    (r"\bdisco\b", "disc"),
    (r"\bdiscos\b", "discs"),
    (r"\bimagen\b", "image"),
    (r"\bim[aá]genes\b", "images"),
    (r"\bficheros\b", "files"),
    (r"\bfichero\b", "file"),
    (r"\bcarpeta\b", "folder"),
    (r"\bl[aá]minas?\b", "sheet"),
    (r"\blogotipo\b", "logo"),
    (r"\bdestino\b", "destination"),
    (r"\barbol_mio\b", "my_tree"),
    (r"\barbol_bueno\b", "reference_tree"),
    (r"\bhace falta\b", "you need"),
    # subcommand names in the usage lines
    (r"\bsacar\b", "extract"),
    (r"\bcotejar\b", "compare"),
    (r"\breparar\b", "repair"),
    (r"\bart[ée]\b", "art"),
    (r"\bver\b", "verify"),
)

_COMPILED_SENTENCES = tuple((re.compile(p, re.I), r) for p, r in _SENTENCES)
_COMPILED_WORDS = tuple((re.compile(p, re.I), r) for p, r in _WORDS)

# Distinctly-Spanish words. Used by the verifier to prove nothing Spanish survives a translation;
# kept short and unambiguous so it does not fire on English words that merely look similar.
_SPANISH_MARKERS = re.compile(
    r"\b(disco|imagen|edicion|europea|hace falta|no puedo|no he podido|no pude|no encuentro|"
    r"sectores|fichero|ficheros|carpeta|lamina|logotipo|tabla de rutas|veredicto|volumen|"
    r"ejecutable|esta vacio|deberia|tama[nñ]o|FALTA|IDENTICO|DISTINTO|bueno|mio)\b", re.I)


def english(text: str) -> str:
    """Translate DiscTool's Spanish output line by line. Unknown text passes through as-is."""
    lines = []
    for line in (text or "").splitlines():
        for pat, rep in _COMPILED_SENTENCES:
            line = pat.sub(rep, line)
        for pat, rep in _COMPILED_WORDS:
            line = pat.sub(rep, line)
        lines.append(line)
    return "\n".join(lines)


def still_spanish(text: str) -> list[str]:
    """Spanish markers left in ``text`` — empty means the translation is clean."""
    return sorted({m.group(0).lower() for m in _SPANISH_MARKERS.finditer(text or "")})
