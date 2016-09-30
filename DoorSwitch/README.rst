How to build this PlatformIO based project
=====================================

1. `Install PlatformIO <http://docs.platformio.org/en/stable/installation.html>`_
2. Clone this repo, git clone https://github.com/jamesleech/ToiletHack.git
3. Create your own user_config.h in the src directory

.. code-block:: bash

    # Change src directory
    > cd ToiletHack\DoorSwitch\src

    # Copy the sample_user_config.h file
    > cp sample_user_config.h user_config.h

4. Edit the values for ssid, password and mqtt server to those in your environment
5. Edit platformio.ini
    
    remove the upload_port ip line for initial serial programming
    
    add the line back in with the ip of your device for OTA programming
    
6. Run these commands:

.. code-block:: bash

    # Change directory to example
    > cd ToiletHack\DoorSwitch

    # Build project
    > platformio run

    # Upload firmware
    > platformio run --target upload

    # Clean build files
    > platformio run --target clean
