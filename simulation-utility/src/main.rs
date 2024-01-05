use std::{time::{Duration, self}, thread, borrow::Borrow, io::{self, BufRead}};

use serialport::SerialPort;


// TODO: Create a command builder or simulation control class to build theses strings in a nicer manner
const COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE: &[u8] = b"CM,3,1";
const COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_FALSE: &[u8] = b"CM,3,0";
const COMMUNICATION_MODULE_OBEC_STATUS_BASE: &[u8] = b"CM,0,001414.125.24.12\0";
const COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE: &[u8] = b"CM,2,010914.125.24.12\0";
const COMMUNICATION_MODULE_OBEC_STATUS_LOW_TANK_VENT_TEMPERATURE: &[u8] = b"CM,2,010905.125.24.12\0";

const COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE: &[u8] = b"CM,2,1";
const COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_FALSE: &[u8] = b"CM,2,0";

const COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_FALSE: &[u8] = b"CM,0,EV0";
const COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_TRUE: &[u8] = b"CM,0,EV1";
const COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING: &[u8] = b"CM,0,SSLDNG";

fn main() {
    let write_port_name = "/dev/cu.usbserial-0001"; // Replace with the correct port
    let baud_rate = 460800;

    // Open the write port
    let mut write_port = serialport::new(write_port_name, baud_rate)
        .timeout(Duration::from_millis(600))
        .open()
        .expect("Failed to open write port");

    let mut reader = io::BufReader::new(write_port);
    let mut line = String::new();

    loop {
        let mut read_buf: Vec<u8> = vec![0; 400]; // Adjust the buffer size as needed
        match reader.read_line(&mut line) {
            Ok(bytes_read) => {
                if bytes_read > 0 {
                    println!("Received from Arduino: {}", line.trim());
                    line.clear();
                }
            }
            Err(e) => eprintln!("Error reading from serial port: {}", e),
        }
    }




    // pressure_abort(write_port);
}

fn pressure_abort(mut write_port: Box<dyn SerialPort>) {
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(&COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
}

fn pressure_warning_and_back_to_standby(mut write_port: Box<dyn SerialPort>) {
    // We shoud also write the value of OBEC_status_data_available, right? Before or after writing the value of OBECStatus_data?
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(5000));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
}

fn pressure_warning_ext_vent_and_back_to_standby_plus_ext_vent_as_default_check(mut write_port: Box<dyn SerialPort>) {
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(25000));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1500));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(5000));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_TRUE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    }

fn loading_test(mut write_port: Box<dyn SerialPort>) {
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING);
    write_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_LOW_TANK_VENT_TEMPERATURE);
    write_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(2000));
    write_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING);
    write_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE);
}