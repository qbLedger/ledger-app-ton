#pragma once

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

#include "types.h"

/**
 * Enumeration for endianness.
 */
typedef enum {
    BE,  /// Big Endian
    LE   /// Little Endian
} endianness_t;

/**
 * Struct for buffer with size and offset.
 */
typedef struct {
    const uint8_t *ptr;  /// Pointer to byte buffer
    size_t size;         /// Size of byte buffer
    size_t offset;       /// Offset in byte buffer
} buffer_t;

/**
 * Tell whether buffer can read bytes or not.
 *
 * @param[in] buffer
 *   Pointer to input buffer struct.
 * @param[in] n
 *   Number of bytes to read in buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_can_read(const buffer_t *buffer, size_t n);

/**
 * Seek the buffer to specific offset.
 *
 * @param[in,out] buffer
 *   Pointer to input buffer struct.
 * @param[in]     offset
 *   Specific offset to seek.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_seek_set(buffer_t *buffer, size_t offset);

/**
 * Seek buffer relatively to current offset.
 *
 * @param[in,out] buffer
 *   Pointer to input buffer struct.
 * @param[in]     offset
 *   Offset to seek relatively to `buffer->offset`.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_seek_cur(buffer_t *buffer, size_t offset);

/**
 * Seek the buffer relatively to the end.
 *
 * @param[in,out] buffer
 *   Pointer to input buffer struct.
 * @param[in]     offset
 *   Offset to seek relatively to `buffer->size`.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_seek_end(buffer_t *buffer, size_t offset);

/**
 * Read 1 byte boolean into bool.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     value
 *   Pointer to 8-bit unsigned integer read from buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_bool(buffer_t *buffer, bool *value);

/**
 * Read 1 byte from buffer into uint8_t.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     value
 *   Pointer to 8-bit unsigned integer read from buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_u8(buffer_t *buffer, uint8_t *value);

/**
 * Read 2 bytes from buffer into uint16_t.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     value
 *   Pointer to 16-bit unsigned integer read from buffer.
 * @param[in]      endianness
 *   Either BE (Big Endian) or LE (Little Endian).
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_u16(buffer_t *buffer, uint16_t *value, endianness_t endianness);

/**
 * Read 4 bytes from buffer into uint32_t.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     value
 *   Pointer to 32-bit unsigned integer read from buffer.
 * @param[in]      endianness
 *   Either BE (Big Endian) or LE (Little Endian).
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_u32(buffer_t *buffer, uint32_t *value, endianness_t endianness);

/**
 * Read 6 bytes from buffer into uint64_t.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     value
 *   Pointer to 64-bit unsigned integer read from buffer.
 * @param[in]      endianness
 *   Either BE (Big Endian) or LE (Little Endian).
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_u48(buffer_t *buffer, uint64_t *value, endianness_t endianness);

/**
 * Read 8 bytes from buffer into uint64_t.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     value
 *   Pointer to 64-bit unsigned integer read from buffer.
 * @param[in]      endianness
 *   Either BE (Big Endian) or LE (Little Endian).
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_u64(buffer_t *buffer, uint64_t *value, endianness_t endianness);

/**
 * Read BIP32 path from buffer.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     out
 *   Pointer to output 32-bit integer buffer.
 * @param[in]      out_len
 *   Number of BIP32 paths read in the output buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_bip32_path(buffer_t *buffer, uint32_t *out, size_t out_len);

/**
 * Copy all remaining bytes from buffer without moving offset.
 *
 * @param[in]  buffer
 *   Pointer to input buffer struct.
 * @param[out] out
 *   Pointer to output byte buffer.
 * @param[in]  out_len
 *   Length of output byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_copy(const buffer_t *buffer, uint8_t *out, size_t out_len);

/**
 * Move all remaining bytes from buffer.
 *
 * @param[in,out]  buffer
 *   Pointer to input buffer struct.
 * @param[out]     out
 *   Pointer to output byte buffer.
 * @param[in]      out_len
 *   Length of output byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_move(buffer_t *buffer, uint8_t *out, size_t out_len);

/**
 * Set out to point to the remaining contents of the buffer.
 *
 * @param[in]  buffer
 *   Pointer to input buffer struct.
 * @param[out] out
 *   Pointer to output byte buffer.
 * @param[in]  out_len
 *   Length of output byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_ref(buffer_t *buffer, uint8_t **out, size_t out_len);

/**
 * Move some bytes from buffer.
 *
 * @param[in]  buffer
 *   Pointer to input buffer struct.
 * @param[out] out
 *   Pointer to output byte buffer.
 * @param[in]  out_len
 *   Length of output byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_buffer(buffer_t *buffer, uint8_t *out, size_t out_len);

/**
 * Read variable length integer from buffer.
 *
 * @param[in]  buffer
 *   Pointer to input buffer struct.
 * @param[out] out_size
 *   Pointer to variable length integer size.
 * @param[out] out
 *   Pointer to output byte buffer.
 * @param[in]  out_len
 *   Length of output byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_varuint(buffer_t *buffer, uint8_t *out_size, uint8_t *out, size_t out_len);

/**
 * Tell how many bytes are left in buffer.
 *
 * @param[in]  buffer
 *   Pointer to input buffer struct.
 *
 * @return amount of bytes left in buffer
 *
 */
size_t buffer_remaining(const buffer_t *buffer);

/**
 * Read serialized address (33 bytes) from buffer.
 *
 * @param[in]  buffer
 *   Pointer to input buffer struct.
 * @param[out] out
 *   Pointer to output address.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_address(buffer_t *buf, address_t *out);

/**
 * Read serialized cell reference (34 bytes) from buffer.
 *
 * @param[in]  buffer
 *   Pointer to input buffer struct.
 * @param[out] out
 *   Pointer to output cell reference.
 *
 * @return true if success, false otherwise.
 *
 */
bool buffer_read_cell_ref(buffer_t *buf, CellRef_t *out);
