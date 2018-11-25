#! /bin/bash

# Los echo que incluyen la hora (HH:mm)
# son para poder hacer testing.

FIFO_CAMARAS="/tmp/camaras"

echo "EAAA999 09:12" >> $FIFO_CAMARAS
sleep 1
echo "EIII111 09:00" >> $FIFO_CAMARAS
sleep 1
echo "SAAA999 14:30" >> $FIFO_CAMARAS
sleep 1
echo "EAAA999 17:40" >> $FIFO_CAMARAS
sleep 1
echo "EBBB000 18:00" >> $FIFO_CAMARAS
sleep 1
echo "ECAB000 18:00" >> $FIFO_CAMARAS
sleep 1
echo "SBBB000 20:00" >> $FIFO_CAMARAS
sleep 1
echo "SAAA999 20:30" >> $FIFO_CAMARAS
sleep 1
echo "EBBB000 21:05" >> $FIFO_CAMARAS
sleep 1
echo "SIII111 22:00" >> $FIFO_CAMARAS
sleep 1