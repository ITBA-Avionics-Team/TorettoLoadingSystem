use std::{time::{Duration, self}, thread, borrow::Borrow, io::{self, BufRead}, sync::{Arc, Mutex, MutexGuard}};

use serialport::SerialPort;


// TODO: Create a command builder or simulation control class to build theses strings in a nicer manner
const COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE: &[u8] = b"CM,3,1|";
const COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_FALSE: &[u8] = b"CM,3,0|";
const COMMUNICATION_MODULE_OBEC_STATUS_BASE: &[u8] = b"CM,0,001414.125.24.12)|";
const COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE: &[u8] = b"CM,2,010914.125.24.12)|";
const COMMUNICATION_MODULE_OBEC_STATUS_LOW_TANK_VENT_TEMPERATURE: &[u8] = b"CM,2,010905.125.24.12)|";

const COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE: &[u8] = b"CM,2,1|";
const COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_FALSE: &[u8] = b"CM,2,0|";

const COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_FALSE: &[u8] = b"CM,0,EV0|";
const COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_TRUE: &[u8] = b"CM,0,EV1|";
const COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING: &[u8] = b"CM,0,SSLDNG|";

fn main() {
    let serial_port_name = "/dev/cu.usbserial-0001"; // Replace with the correct port
    let baud_rate = 115200;

    // Open the write port
    let mut serial_port = serialport::new(serial_port_name, baud_rate)
        .timeout(Duration::from_millis(1000))
        .open()
        .expect("Failed to open write port");

    let serial_port_arc = Arc::new(Mutex::new(serial_port));

    let reader_port = Arc::clone(&serial_port_arc);
    thread::spawn(move || {
        println!("Spawned thread for reading serial port");
        loop {
            let mut port = reader_port.lock().unwrap();
            let mut reader = io::BufReader::new(&mut *port);
            let mut line = String::new();
            match reader.read_line(&mut line) {
                Ok(bytes_read) => {
                    if bytes_read > 0 {
                        println!("Received from Arduino: {}", line.trim());
                        line.clear();
                    }
                }
                Err(e) => {
                    // eprintln!("Error reading from serial port: {}", e)
                },
            }
            drop(port);
            thread::sleep(time::Duration::from_millis(50));        
        }
    });

    thread::sleep(time::Duration::from_millis(2000));

    pressure_abort(serial_port_arc);
    loop {};

}

fn pressure_abort(mut serial_port_mutex: Arc<Mutex<Box<dyn SerialPort>>>) {
    let mut serial_port = serial_port_mutex.lock().unwrap();
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    println!("Wrote base OBEC Status to serial port.");
    drop(serial_port);
    thread::sleep(time::Duration::from_millis(1000));
    let mut serial_port = serial_port_mutex.lock().unwrap();
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    println!("Wrote \"OBEC Status available\" signal to serial port.");
    drop(serial_port);
    thread::sleep(time::Duration::from_millis(5000));

    let mut serial_port = serial_port_mutex.lock().unwrap();
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    println!("Wrote base OBEC Status to serial port.");
    drop(serial_port);
    thread::sleep(time::Duration::from_millis(1000));
    let mut serial_port = serial_port_mutex.lock().unwrap();
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    println!("Wrote \"OBEC Status available\" signal to serial port.");
    drop(serial_port);
    thread::sleep(time::Duration::from_millis(5000));

    serial_port = serial_port_mutex.lock().unwrap();
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    println!("Wrote high tank pressure OBEC Status to serial port.");
    drop(serial_port);
    thread::sleep(time::Duration::from_millis(1000));
    let mut serial_port = serial_port_mutex.lock().unwrap();
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    println!("Wrote \"OBEC Status available\" signal to serial port.");
    drop(serial_port);
}

fn pressure_warning_and_back_to_standby(mut serial_port: Box<dyn SerialPort>) {
    // We shoud also write the value of OBEC_status_data_available, right? Before or after writing the value of OBECStatus_data?
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(5000));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
}

fn pressure_warning_ext_vent_and_back_to_standby_plus_ext_vent_as_default_check(mut serial_port: Box<dyn SerialPort>) {
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(25000));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1500));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(5000));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    serial_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_EXT_VENT_AS_DEFAULT_TRUE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_HIGH_PRESSURE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    }

fn loading_test(mut serial_port: Box<dyn SerialPort>) {
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_BASE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    serial_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING);
    serial_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(1000));
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_LOW_TANK_VENT_TEMPERATURE);
    serial_port.write_all(COMMUNICATION_MODULE_OBEC_STATUS_AVAILABLE_TRUE);
    thread::sleep(time::Duration::from_millis(2000));
    serial_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_SWITCH_STATE_LOADING);
    serial_port.write_all(COMMUNICATION_MODULE_MCC_COMMAND_AVAILABLE_TRUE);
}