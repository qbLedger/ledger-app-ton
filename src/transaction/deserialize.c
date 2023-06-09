/*****************************************************************************
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include "deserialize.h"
#include "types.h"
#include "../common/buffer.h"
#include "hash.h"
#include "cell.h"
#include "hints.h"
#include "../constants.h"

#define SAFE(RES, CODE) \
    if (!RES) {         \
        return CODE;    \
    }

bool buffer_read_address(buffer_t *buf, address_t *out) {
    if (!buffer_read_u8(buf, &out->chain)) {
        return false;
    }
    if (!buffer_read_buffer(buf, out->hash, HASH_LEN)) {
        return false;
    }
    return true;
}

bool buffer_read_cell_ref(buffer_t *buf, CellRef_t *out) {
    if (!buffer_read_u16(buf, &out->max_depth, BE)) {
        return false;
    }
    if (!buffer_read_buffer(buf, out->hash, HASH_LEN)) {
        return false;
    }
    return true;
}

parser_status_e transaction_deserialize(buffer_t *buf, transaction_t *tx) {
    if (buf->size > MAX_TX_LEN) {
        return WRONG_LENGTH_ERROR;
    }

    // tag
    SAFE(buffer_read_u8(buf, &tx->tag), TAG_PARSING_ERROR);
    if (tx->tag != 0x00) {  // Only 0x00 is supported now
        return TAG_PARSING_ERROR;
    }

    // Basic Transaction parameters
    SAFE(buffer_read_u32(buf, &tx->seqno, BE), SEQ_PARSING_ERROR);
    SAFE(buffer_read_u32(buf, &tx->timeout, BE), TIMEOUT_PARSING_ERROR);
    SAFE(buffer_read_varuint(buf, &tx->value_len, tx->value_buf, MAX_VALUE_BYTES_LEN),
         VALUE_PARSING_ERROR);
    SAFE(buffer_read_address(buf, &tx->to), TO_PARSING_ERROR);
    SAFE(buffer_read_u8(buf, &tx->bounce), BOUNCE_PARSING_ERROR);
    SAFE(buffer_read_u8(buf, &tx->send_mode), SEND_MODE_PARSING_ERROR);

    // state-init
    SAFE(buffer_read_bool(buf, &tx->has_state_init), STATE_INIT_PARSING_ERROR);
    if (tx->has_state_init) {
        SAFE(buffer_read_cell_ref(buf, &tx->state_init), STATE_INIT_PARSING_ERROR);
    }

    // Payload
    SAFE(buffer_read_bool(buf, &tx->has_payload), PAYLOAD_PARSING_ERROR);
    if (tx->has_payload) {
        SAFE(buffer_read_cell_ref(buf, &tx->payload), STATE_INIT_PARSING_ERROR);
    }

    // Hints
    SAFE(buffer_read_bool(buf, &tx->has_hints), HINTS_PARSING_ERROR);
    if (tx->has_hints) {
        if (!tx->has_payload) {
            return HINTS_PARSING_ERROR;
        }
        SAFE(buffer_read_u32(buf, &tx->hints_type, BE), HINTS_PARSING_ERROR);
        SAFE(buffer_read_u16(buf, &tx->hints_len, BE), HINTS_PARSING_ERROR);
        SAFE(buffer_read_ref(buf, &tx->hints_data, tx->hints_len), HINTS_PARSING_ERROR);
    }

    // Process hints
    SAFE(process_hints(tx), HINTS_PARSING_ERROR);

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
