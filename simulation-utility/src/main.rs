use std::{time::{Duration, self}, thread};


// TODO: Create a command builder or simulation control class to build theses strings in a nicer manner
const COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE = "CM,3,1";
const COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_FALSE = "CM,3,0";
const COMMUNICATION_MODULE_OBEC_STATUS_BASE = "CM,0,001414.125.24.12\0";
const COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE = "CM,2,010914.125.24.12\0";
const COMMUNICATION_MODULE_OBEC_STATUS_LOW_TANK_VENT_TEMPERATURE = "CM,2,010905.125.24.12\0";

const COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE = "CM,2,1";
const COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_FALSE = "CM,2,0";

const COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_FALSE = "CM,0,EV0";
const COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_TRUE = "CM,0,EV1";
const COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING = "CM,0,SSLDNG";

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
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    
    write_port.close().expect("Failed to close write port");
}

fn pressure_warning_and_back_to_standby(&mut write_port) {
    // We shoud also write the value of OBEC_status_data_available, right? Before or after writing the value of OBECStatus_data?
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(5000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);

    write_port.close().expect("Failed to close write port");
}

fn pressure_warning_ext_vent_and_back_to_standby_plus_ext_vent_as_default_check(&mut write_port) {
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(25000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1500));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(5000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&EXT_VENT_AS_DEFAULT_MSG);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    
    write_port.close().expect("Failed to close write port");
}

fn loading_test(&mut write_port) {
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING);
    write_port.write_all(&COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_LOW_TANK_VENT_TEMPERATURE);
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(2000));
    write_port.write_all(&COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING);
    write_port.write_all(&COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE);
}