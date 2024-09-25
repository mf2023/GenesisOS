/*
 * Copyright (c) 2024 Wenze Wei
 *
 * This file is part of GenesisOS.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License, v. 2.0.
 * You can obtain a copy of the GPL at
 * https://www.gnu.org/licenses/old-licenses/gpl-2.0.html.
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
 * the specific language governing rights and limitations under the License.
 */

use sha2::{Sha256, Digest};
use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use rand::Rng;


#[no_mangle]
pub extern "C" fn uid_generate() -> u32 {
    let mut rng = rand::thread_rng();
    rng.gen_range(10000..=99999)
}


#[no_mangle]
pub extern "C" fn sha256_hash_c_string(input: *const c_char) -> *mut c_char {
    if input.is_null() {
        return std::ptr::null_mut();
    }

    unsafe {
        let c_str = CStr::from_ptr(input);
        let input_str = match c_str.to_str() {
            Ok(s) => s,
            Err(_) => return std::ptr::null_mut(),
        };

        let mut hasher = Sha256::new();
        hasher.update(input_str.as_bytes());
        let result = hasher.finalize();
        let hex_result = format!("{:x}", result);
        let c_result = CString::new(hex_result).unwrap();

        c_result.into_raw()
    }
}