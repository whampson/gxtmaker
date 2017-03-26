#ifndef _COMPILER_H_
#define _COMPILER_H_

/*
 * Translates the specified GXT source file into a proper GXT file.
 *
 * @param src_file the path to the source file
 * @param out_file the path to the compiled file
 *
 * @return 0 if compilation was successful, nozero if unsuccessful
 */
int compile(const char *src_file, const char *out_file);

#endif /* _COMPILER_H_ */
