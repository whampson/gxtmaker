#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H_

/*
 * Assembles the specified GXT source file into a proper GXT file.
 *
 * @param src_file the path to the source file
 * @param out_file the path to the assembled file
 *
 * @return 0 if assembly was successful, nozero if assembly unsuccessful
 */
int assemble(const char *src_file, const char *out_file);

#endif /* _ASSEMBLER_H_ */
