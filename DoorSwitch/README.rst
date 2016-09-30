How to build this PlatformIO based project
=====================================

1. `Install PlatformIO <http://docs.platformio.org/en/stable/installation.html>`_
2. Clone this repo, git clone https://github.com/jamesleech/ToiletHack.git`_
3. Run these commands: _

.. code-block:: bash

    # Change directory to example
    > cd ToiletHack

    # Build project
    > platformio run

    # Upload firmware
    > platformio run --target upload

    # Clean build files
    > platformio run --target clean
