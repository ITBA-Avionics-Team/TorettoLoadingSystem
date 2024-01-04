use std::{time::{Duration, self}, thread};

const BASE_OBEC_STATUS_DATA_MSG = "CM,0,001410.125.24.12\0";
const OBEC_STATUS_DATA_HIGH_PRESSURE_MSG = "CM,2,010910.125.24.12\0";

const EXT_VENT_AS_DEFAULT_MSG = "CM,2,EV0";

fn main() {
    let write_port_name = "/dev/ttys003"; // Replace with the correct port
    let baud_rate = 19200;

    // Open the write port
    let mut write_port = serialport::new(write_port_name, baud_rate)
        .timeout(Duration::from_millis(10))
        .open()
        .expect("Failed to open write port");


    let buff: [u8; 28] = [b'0', b'0', b'1', b'4', b'1', b'0', b'.', b'1', b'2', b'5', b'.', b'2', b'0', b'0', b'1', b'4', b'4', b'.', b'1', b'2', b'4', b'.', b'2', b'0', b'?', b'0', b'0', b'8'];
    pressure_abort(write_port);
}

fn pressure_abort(&mut write_port) {
    write_port.write_all(&BASE_OBEC_STATUS_DATA_MSG);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&OBEC_STATUS_DATA_HIGH_PRESSURE_MSG);
    
    write_port.close().expect("Failed to close write port");
}

fn pressure_warning_and_back_to_standby(&mut write_port) {
    // We shoud also write the value of OBEC_status_data_available, right? Before or after writing the value of OBECStatus_data?
    write_port.write_all(&BASE_OBEC_STATUS_DATA_MSG);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&OBEC_STATUS_DATA_HIGH_PRESSURE_MSG);
    thread::sleep(time::Duration::from_millis(5000));
    write_port.write_all(&BASE_OBEC_STATUS_DATA_MSG);

    write_port.close().expect("Failed to close write port");
}

fn pressure_warning_ext_vent_and_back_to_standby_plus_ext_vent_as_default_check(&mut write_port) {
    write_port.write_all(&BASE_OBEC_STATUS_DATA_MSG);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&OBEC_STATUS_DATA_HIGH_PRESSURE_MSG);
    thread::sleep(time::Duration::from_millis(25000));
    write_port.write_all(&BASE_OBEC_STATUS_DATA_MSG);
    thread::sleep(time::Duration::from_millis(1500));
    write_port.write_all(&OBEC_STATUS_DATA_HIGH_PRESSURE_MSG);
    thread::sleep(time::Duration::from_millis(5000));
    write_port.write_all(&BASE_OBEC_STATUS_DATA_MSG);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&EXT_VENT_AS_DEFAULT_MSG);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&OBEC_STATUS_DATA_HIGH_PRESSURE_MSG);
    
    write_port.close().expect("Failed to close write port");
}