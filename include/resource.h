/*
 * Symbols for interacting with resources
 */
#ifndef _RESOURCE_H_
#define _RESOURCE_H_

extern char _binary_res_keymap_txt_start[];
extern char _binary_res_keymap_txt_end[];
extern char _binary_res_keymap_txt_size[];

#define res_keymap_start_ptr _binary_res_keymap_txt_start
#define res_keymap_end_ptr _binary_res_keymap_txt_end
#define res_keymap_size ((size_t)_binary_res_keymap_txt_size)

#endif  // _RESOURCE_H_

