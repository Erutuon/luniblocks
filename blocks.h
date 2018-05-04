#ifndef BLOCKS_H
#define _BLOCKS_H

#include <stdio.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

/* Format for printed codepoint */
#ifdef ZEROPADHEX
#define UNIFORMAT  "%0*X"
#else
#define UNIFORMAT  "%*X"
#endif
#define GAP        "  "
#define PROMPT     "> "

typedef unsigned char  uchar;
typedef unsigned short ushrt;
typedef unsigned       uint;

typedef struct Block {
	char const * const name;
	const uint low;
	const uint high;
} Block;

/*
 *  http://www.unicode.org/Public/UNIDATA/Blocks.txt
 *  name of block, lowest codepoint, highest codepoint
 */
Block blocks[] = {
	{ "Basic Latin",                                      0x000000, 0x00007F },
	{ "Latin-1 Supplement",                               0x000080, 0x0000FF },
	{ "Latin Extended-A",                                 0x000100, 0x00017F },
	{ "Latin Extended-B",                                 0x000180, 0x00024F },
	{ "IPA Extensions",                                   0x000250, 0x0002AF },
	{ "Spacing Modifier Letters",                         0x0002B0, 0x0002FF },
	{ "Combining Diacritical Marks",                      0x000300, 0x00036F },
	{ "Greek and Coptic",                                 0x000370, 0x0003FF },
	{ "Cyrillic",                                         0x000400, 0x0004FF },
	{ "Cyrillic Supplement",                              0x000500, 0x00052F },
	{ "Armenian",                                         0x000530, 0x00058F },
	{ "Hebrew",                                           0x000590, 0x0005FF },
	{ "Arabic",                                           0x000600, 0x0006FF },
	{ "Syriac",                                           0x000700, 0x00074F },
	{ "Arabic Supplement",                                0x000750, 0x00077F },
	{ "Thaana",                                           0x000780, 0x0007BF },
	{ "NKo",                                              0x0007C0, 0x0007FF },
	{ "Samaritan",                                        0x000800, 0x00083F },
	{ "Mandaic",                                          0x000840, 0x00085F },
	{ "Syriac Supplement",                                0x000860, 0x00086F },
	{ "Arabic Extended-A",                                0x0008A0, 0x0008FF },
	{ "Devanagari",                                       0x000900, 0x00097F },
	{ "Bengali",                                          0x000980, 0x0009FF },
	{ "Gurmukhi",                                         0x000A00, 0x000A7F },
	{ "Gujarati",                                         0x000A80, 0x000AFF },
	{ "Oriya",                                            0x000B00, 0x000B7F },
	{ "Tamil",                                            0x000B80, 0x000BFF },
	{ "Telugu",                                           0x000C00, 0x000C7F },
	{ "Kannada",                                          0x000C80, 0x000CFF },
	{ "Malayalam",                                        0x000D00, 0x000D7F },
	{ "Sinhala",                                          0x000D80, 0x000DFF },
	{ "Thai",                                             0x000E00, 0x000E7F },
	{ "Lao",                                              0x000E80, 0x000EFF },
	{ "Tibetan",                                          0x000F00, 0x000FFF },
	{ "Myanmar",                                          0x001000, 0x00109F },
	{ "Georgian",                                         0x0010A0, 0x0010FF },
	{ "Hangul Jamo",                                      0x001100, 0x0011FF },
	{ "Ethiopic",                                         0x001200, 0x00137F },
	{ "Ethiopic Supplement",                              0x001380, 0x00139F },
	{ "Cherokee",                                         0x0013A0, 0x0013FF },
	{ "Unified Canadian Aboriginal Syllabics",            0x001400, 0x00167F },
	{ "Ogham",                                            0x001680, 0x00169F },
	{ "Runic",                                            0x0016A0, 0x0016FF },
	{ "Tagalog",                                          0x001700, 0x00171F },
	{ "Hanunoo",                                          0x001720, 0x00173F },
	{ "Buhid",                                            0x001740, 0x00175F },
	{ "Tagbanwa",                                         0x001760, 0x00177F },
	{ "Khmer",                                            0x001780, 0x0017FF },
	{ "Mongolian",                                        0x001800, 0x0018AF },
	{ "Unified Canadian Aboriginal Syllabics Extended",   0x0018B0, 0x0018FF },
	{ "Limbu",                                            0x001900, 0x00194F },
	{ "Tai Le",                                           0x001950, 0x00197F },
	{ "New Tai Lue",                                      0x001980, 0x0019DF },
	{ "Khmer Symbols",                                    0x0019E0, 0x0019FF },
	{ "Buginese",                                         0x001A00, 0x001A1F },
	{ "Tai Tham",                                         0x001A20, 0x001AAF },
	{ "Combining Diacritical Marks Extended",             0x001AB0, 0x001AFF },
	{ "Balinese",                                         0x001B00, 0x001B7F },
	{ "Sundanese",                                        0x001B80, 0x001BBF },
	{ "Batak",                                            0x001BC0, 0x001BFF },
	{ "Lepcha",                                           0x001C00, 0x001C4F },
	{ "Ol Chiki",                                         0x001C50, 0x001C7F },
	{ "Cyrillic Extended-C",                              0x001C80, 0x001C8F },
	{ "Sundanese Supplement",                             0x001CC0, 0x001CCF },
	{ "Vedic Extensions",                                 0x001CD0, 0x001CFF },
	{ "Phonetic Extensions",                              0x001D00, 0x001D7F },
	{ "Phonetic Extensions Supplement",                   0x001D80, 0x001DBF },
	{ "Combining Diacritical Marks Supplement",           0x001DC0, 0x001DFF },
	{ "Latin Extended Additional",                        0x001E00, 0x001EFF },
	{ "Greek Extended",                                   0x001F00, 0x001FFF },
	{ "General Punctuation",                              0x002000, 0x00206F },
	{ "Superscripts and Subscripts",                      0x002070, 0x00209F },
	{ "Currency Symbols",                                 0x0020A0, 0x0020CF },
	{ "Combining Diacritical Marks for Symbols",          0x0020D0, 0x0020FF },
	{ "Letterlike Symbols",                               0x002100, 0x00214F },
	{ "Number Forms",                                     0x002150, 0x00218F },
	{ "Arrows",                                           0x002190, 0x0021FF },
	{ "Mathematical Operators",                           0x002200, 0x0022FF },
	{ "Miscellaneous Technical",                          0x002300, 0x0023FF },
	{ "Control Pictures",                                 0x002400, 0x00243F },
	{ "Optical Character Recognition",                    0x002440, 0x00245F },
	{ "Enclosed Alphanumerics",                           0x002460, 0x0024FF },
	{ "Box Drawing",                                      0x002500, 0x00257F },
	{ "Block Elements",                                   0x002580, 0x00259F },
	{ "Geometric Shapes",                                 0x0025A0, 0x0025FF },
	{ "Miscellaneous Symbols",                            0x002600, 0x0026FF },
	{ "Dingbats",                                         0x002700, 0x0027BF },
	{ "Miscellaneous Mathematical Symbols-A",             0x0027C0, 0x0027EF },
	{ "Supplemental Arrows-A",                            0x0027F0, 0x0027FF },
	{ "Braille Patterns",                                 0x002800, 0x0028FF },
	{ "Supplemental Arrows-B",                            0x002900, 0x00297F },
	{ "Miscellaneous Mathematical Symbols-B",             0x002980, 0x0029FF },
	{ "Supplemental Mathematical Operators",              0x002A00, 0x002AFF },
	{ "Miscellaneous Symbols and Arrows",                 0x002B00, 0x002BFF },
	{ "Glagolitic",                                       0x002C00, 0x002C5F },
	{ "Latin Extended-C",                                 0x002C60, 0x002C7F },
	{ "Coptic",                                           0x002C80, 0x002CFF },
	{ "Georgian Supplement",                              0x002D00, 0x002D2F },
	{ "Tifinagh",                                         0x002D30, 0x002D7F },
	{ "Ethiopic Extended",                                0x002D80, 0x002DDF },
	{ "Cyrillic Extended-A",                              0x002DE0, 0x002DFF },
	{ "Supplemental Punctuation",                         0x002E00, 0x002E7F },
	{ "CJK Radicals Supplement",                          0x002E80, 0x002EFF },
	{ "Kangxi Radicals",                                  0x002F00, 0x002FDF },
	{ "Ideographic Description Characters",               0x002FF0, 0x002FFF },
	{ "CJK Symbols and Punctuation",                      0x003000, 0x00303F },
	{ "Hiragana",                                         0x003040, 0x00309F },
	{ "Katakana",                                         0x0030A0, 0x0030FF },
	{ "Bopomofo",                                         0x003100, 0x00312F },
	{ "Hangul Compatibility Jamo",                        0x003130, 0x00318F },
	{ "Kanbun",                                           0x003190, 0x00319F },
	{ "Bopomofo Extended",                                0x0031A0, 0x0031BF },
	{ "CJK Strokes",                                      0x0031C0, 0x0031EF },
	{ "Katakana Phonetic Extensions",                     0x0031F0, 0x0031FF },
	{ "Enclosed CJK Letters and Months",                  0x003200, 0x0032FF },
	{ "CJK Compatibility",                                0x003300, 0x0033FF },
	{ "CJK Unified Ideographs Extension A",               0x003400, 0x004DBF },
	{ "Yijing Hexagram Symbols",                          0x004DC0, 0x004DFF },
	{ "CJK Unified Ideographs",                           0x004E00, 0x009FFF },
	{ "Yi Syllables",                                     0x00A000, 0x00A48F },
	{ "Yi Radicals",                                      0x00A490, 0x00A4CF },
	{ "Lisu",                                             0x00A4D0, 0x00A4FF },
	{ "Vai",                                              0x00A500, 0x00A63F },
	{ "Cyrillic Extended-B",                              0x00A640, 0x00A69F },
	{ "Bamum",                                            0x00A6A0, 0x00A6FF },
	{ "Modifier Tone Letters",                            0x00A700, 0x00A71F },
	{ "Latin Extended-D",                                 0x00A720, 0x00A7FF },
	{ "Syloti Nagri",                                     0x00A800, 0x00A82F },
	{ "Common Indic Number Forms",                        0x00A830, 0x00A83F },
	{ "Phags-pa",                                         0x00A840, 0x00A87F },
	{ "Saurashtra",                                       0x00A880, 0x00A8DF },
	{ "Devanagari Extended",                              0x00A8E0, 0x00A8FF },
	{ "Kayah Li",                                         0x00A900, 0x00A92F },
	{ "Rejang",                                           0x00A930, 0x00A95F },
	{ "Hangul Jamo Extended-A",                           0x00A960, 0x00A97F },
	{ "Javanese",                                         0x00A980, 0x00A9DF },
	{ "Myanmar Extended-B",                               0x00A9E0, 0x00A9FF },
	{ "Cham",                                             0x00AA00, 0x00AA5F },
	{ "Myanmar Extended-A",                               0x00AA60, 0x00AA7F },
	{ "Tai Viet",                                         0x00AA80, 0x00AADF },
	{ "Meetei Mayek Extensions",                          0x00AAE0, 0x00AAFF },
	{ "Ethiopic Extended-A",                              0x00AB00, 0x00AB2F },
	{ "Latin Extended-E",                                 0x00AB30, 0x00AB6F },
	{ "Cherokee Supplement",                              0x00AB70, 0x00ABBF },
	{ "Meetei Mayek",                                     0x00ABC0, 0x00ABFF },
	{ "Hangul Syllables",                                 0x00AC00, 0x00D7AF },
	{ "Hangul Jamo Extended-B",                           0x00D7B0, 0x00D7FF },
	{ "High Surrogates",                                  0x00D800, 0x00DB7F },
	{ "High Private Use Surrogates",                      0x00DB80, 0x00DBFF },
	{ "Low Surrogates",                                   0x00DC00, 0x00DFFF },
	{ "Private Use Area",                                 0x00E000, 0x00F8FF },
	{ "CJK Compatibility Ideographs",                     0x00F900, 0x00FAFF },
	{ "Alphabetic Presentation Forms",                    0x00FB00, 0x00FB4F },
	{ "Arabic Presentation Forms-A",                      0x00FB50, 0x00FDFF },
	{ "Variation Selectors",                              0x00FE00, 0x00FE0F },
	{ "Vertical Forms",                                   0x00FE10, 0x00FE1F },
	{ "Combining Half Marks",                             0x00FE20, 0x00FE2F },
	{ "CJK Compatibility Forms",                          0x00FE30, 0x00FE4F },
	{ "Small Form Variants",                              0x00FE50, 0x00FE6F },
	{ "Arabic Presentation Forms-B",                      0x00FE70, 0x00FEFF },
	{ "Halfwidth and Fullwidth Forms",                    0x00FF00, 0x00FFEF },
	{ "Specials",                                         0x00FFF0, 0x00FFFF },
	{ "Linear B Syllabary",                               0x010000, 0x01007F },
	{ "Linear B Ideograms",                               0x010080, 0x0100FF },
	{ "Aegean Numbers",                                   0x010100, 0x01013F },
	{ "Ancient Greek Numbers",                            0x010140, 0x01018F },
	{ "Ancient Symbols",                                  0x010190, 0x0101CF },
	{ "Phaistos Disc",                                    0x0101D0, 0x0101FF },
	{ "Lycian",                                           0x010280, 0x01029F },
	{ "Carian",                                           0x0102A0, 0x0102DF },
	{ "Coptic Epact Numbers",                             0x0102E0, 0x0102FF },
	{ "Old Italic",                                       0x010300, 0x01032F },
	{ "Gothic",                                           0x010330, 0x01034F },
	{ "Old Permic",                                       0x010350, 0x01037F },
	{ "Ugaritic",                                         0x010380, 0x01039F },
	{ "Old Persian",                                      0x0103A0, 0x0103DF },
	{ "Deseret",                                          0x010400, 0x01044F },
	{ "Shavian",                                          0x010450, 0x01047F },
	{ "Osmanya",                                          0x010480, 0x0104AF },
	{ "Osage",                                            0x0104B0, 0x0104FF },
	{ "Elbasan",                                          0x010500, 0x01052F },
	{ "Caucasian Albanian",                               0x010530, 0x01056F },
	{ "Linear A",                                         0x010600, 0x01077F },
	{ "Cypriot Syllabary",                                0x010800, 0x01083F },
	{ "Imperial Aramaic",                                 0x010840, 0x01085F },
	{ "Palmyrene",                                        0x010860, 0x01087F },
	{ "Nabataean",                                        0x010880, 0x0108AF },
	{ "Hatran",                                           0x0108E0, 0x0108FF },
	{ "Phoenician",                                       0x010900, 0x01091F },
	{ "Lydian",                                           0x010920, 0x01093F },
	{ "Meroitic Hieroglyphs",                             0x010980, 0x01099F },
	{ "Meroitic Cursive",                                 0x0109A0, 0x0109FF },
	{ "Kharoshthi",                                       0x010A00, 0x010A5F },
	{ "Old South Arabian",                                0x010A60, 0x010A7F },
	{ "Old North Arabian",                                0x010A80, 0x010A9F },
	{ "Manichaean",                                       0x010AC0, 0x010AFF },
	{ "Avestan",                                          0x010B00, 0x010B3F },
	{ "Inscriptional Parthian",                           0x010B40, 0x010B5F },
	{ "Inscriptional Pahlavi",                            0x010B60, 0x010B7F },
	{ "Psalter Pahlavi",                                  0x010B80, 0x010BAF },
	{ "Old Turkic",                                       0x010C00, 0x010C4F },
	{ "Old Hungarian",                                    0x010C80, 0x010CFF },
	{ "Rumi Numeral Symbols",                             0x010E60, 0x010E7F },
	{ "Brahmi",                                           0x011000, 0x01107F },
	{ "Kaithi",                                           0x011080, 0x0110CF },
	{ "Sora Sompeng",                                     0x0110D0, 0x0110FF },
	{ "Chakma",                                           0x011100, 0x01114F },
	{ "Mahajani",                                         0x011150, 0x01117F },
	{ "Sharada",                                          0x011180, 0x0111DF },
	{ "Sinhala Archaic Numbers",                          0x0111E0, 0x0111FF },
	{ "Khojki",                                           0x011200, 0x01124F },
	{ "Multani",                                          0x011280, 0x0112AF },
	{ "Khudawadi",                                        0x0112B0, 0x0112FF },
	{ "Grantha",                                          0x011300, 0x01137F },
	{ "Newa",                                             0x011400, 0x01147F },
	{ "Tirhuta",                                          0x011480, 0x0114DF },
	{ "Siddham",                                          0x011580, 0x0115FF },
	{ "Modi",                                             0x011600, 0x01165F },
	{ "Mongolian Supplement",                             0x011660, 0x01167F },
	{ "Takri",                                            0x011680, 0x0116CF },
	{ "Ahom",                                             0x011700, 0x01173F },
	{ "Warang Citi",                                      0x0118A0, 0x0118FF },
	{ "Zanabazar Square",                                 0x011A00, 0x011A4F },
	{ "Soyombo",                                          0x011A50, 0x011AAF },
	{ "Pau Cin Hau",                                      0x011AC0, 0x011AFF },
	{ "Bhaiksuki",                                        0x011C00, 0x011C6F },
	{ "Marchen",                                          0x011C70, 0x011CBF },
	{ "Masaram Gondi",                                    0x011D00, 0x011D5F },
	{ "Cuneiform",                                        0x012000, 0x0123FF },
	{ "Cuneiform Numbers and Punctuation",                0x012400, 0x01247F },
	{ "Early Dynastic Cuneiform",                         0x012480, 0x01254F },
	{ "Egyptian Hieroglyphs",                             0x013000, 0x01342F },
	{ "Anatolian Hieroglyphs",                            0x014400, 0x01467F },
	{ "Bamum Supplement",                                 0x016800, 0x016A3F },
	{ "Mro",                                              0x016A40, 0x016A6F },
	{ "Bassa Vah",                                        0x016AD0, 0x016AFF },
	{ "Pahawh Hmong",                                     0x016B00, 0x016B8F },
	{ "Miao",                                             0x016F00, 0x016F9F },
	{ "Ideographic Symbols and Punctuation",              0x016FE0, 0x016FFF },
	{ "Tangut",                                           0x017000, 0x0187FF },
	{ "Tangut Components",                                0x018800, 0x018AFF },
	{ "Kana Supplement",                                  0x01B000, 0x01B0FF },
	{ "Kana Extended-A",                                  0x01B100, 0x01B12F },
	{ "Nushu",                                            0x01B170, 0x01B2FF },
	{ "Duployan",                                         0x01BC00, 0x01BC9F },
	{ "Shorthand Format Controls",                        0x01BCA0, 0x01BCAF },
	{ "Byzantine Musical Symbols",                        0x01D000, 0x01D0FF },
	{ "Musical Symbols",                                  0x01D100, 0x01D1FF },
	{ "Ancient Greek Musical Notation",                   0x01D200, 0x01D24F },
	{ "Tai Xuan Jing Symbols",                            0x01D300, 0x01D35F },
	{ "Counting Rod Numerals",                            0x01D360, 0x01D37F },
	{ "Mathematical Alphanumeric Symbols",                0x01D400, 0x01D7FF },
	{ "Sutton SignWriting",                               0x01D800, 0x01DAAF },
	{ "Glagolitic Supplement",                            0x01E000, 0x01E02F },
	{ "Mende Kikakui",                                    0x01E800, 0x01E8DF },
	{ "Adlam",                                            0x01E900, 0x01E95F },
	{ "Arabic Mathematical Alphabetic Symbols",           0x01EE00, 0x01EEFF },
	{ "Mahjong Tiles",                                    0x01F000, 0x01F02F },
	{ "Domino Tiles",                                     0x01F030, 0x01F09F },
	{ "Playing Cards",                                    0x01F0A0, 0x01F0FF },
	{ "Enclosed Alphanumeric Supplement",                 0x01F100, 0x01F1FF },
	{ "Enclosed Ideographic Supplement",                  0x01F200, 0x01F2FF },
	{ "Miscellaneous Symbols and Pictographs",            0x01F300, 0x01F5FF },
	{ "Emoticons",                                        0x01F600, 0x01F64F },
	{ "Ornamental Dingbats",                              0x01F650, 0x01F67F },
	{ "Transport and Map Symbols",                        0x01F680, 0x01F6FF },
	{ "Alchemical Symbols",                               0x01F700, 0x01F77F },
	{ "Geometric Shapes Extended",                        0x01F780, 0x01F7FF },
	{ "Supplemental Arrows-C",                            0x01F800, 0x01F8FF },
	{ "Supplemental Symbols and Pictographs",             0x01F900, 0x01F9FF },
	{ "CJK Unified Ideographs Extension B",               0x020000, 0x02A6DF },
	{ "CJK Unified Ideographs Extension C",               0x02A700, 0x02B73F },
	{ "CJK Unified Ideographs Extension D",               0x02B740, 0x02B81F },
	{ "CJK Unified Ideographs Extension E",               0x02B820, 0x02CEAF },
	{ "CJK Unified Ideographs Extension F",               0x02CEB0, 0x02EBEF },
	{ "CJK Compatibility Ideographs Supplement",          0x02F800, 0x02FA1F },
	{ "Tags",                                             0x0E0000, 0x0E007F },
	{ "Variation Selectors Supplement",                   0x0E0100, 0x0E01EF },
	{ "Supplementary Private Use Area-A",                 0x0F0000, 0x0FFFFF },
	{ "Supplementary Private Use Area-B",                 0x100000, 0x10FFFF },
};

#define BLOCKCOUNT		sizeof blocks / sizeof(Block)

/* an array large enough to hold pointers to all Block structs as well as a NULL pointer */
typedef const Block * Block_Results[BLOCKCOUNT + 1];

/* "Unified Canadian Aboriginal Syllabics Extended", plus null terminator */
#define MAXBLOCKNAME  46 + 1

extern void    print_block     (const Block * const b, uchar name_width, uchar hex_width);

extern void    show_blocks     (Block_Results results);

extern Block * find_next_block (const char lower_query[MAXBLOCKNAME], ushrt * pos);

extern ushrt   find_blocks     (const char * query, Block_Results results);

#endif