/// NotMyFault - random crashtest
/// the name was totally not in the
/// windows tool :P
#![no_std]

unsafe extern "C" {
    fn divide_by_zero();
    fn invalid_opcode();
    fn breakpoint();
    fn double_fault();
}

static mut RNG_STATE: u32 = 0x1234_5678;

fn next_random() -> u32 {
    unsafe {
        let mut x = RNG_STATE;

        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;

        RNG_STATE = x;
        x
    }
}

#[no_mangle]
pub unsafe extern "C" fn run_random_crashtest() {
    let index = next_random() % 4;

    unsafe {
        match index {
            0 => divide_by_zero(),
            1 => invalid_opcode(),
            2 => breakpoint(),
            _ => double_fault(),
        }
    }
}
