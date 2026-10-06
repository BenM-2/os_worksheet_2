#!/bin/bash

DELAY=1  # Seconds to wait between commands (adjust as needed)
 
(
  sleep $DELAY  # Wait for command output
  echo 'q'
) | telnet localhost 45454