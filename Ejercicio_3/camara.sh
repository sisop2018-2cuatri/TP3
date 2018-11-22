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
echo "EBB000ZZ 21:05" >> $FIFO_CAMARAS
sleep 1
echo "SIII111 22:00" >> $FIFO_CAMARAS
sleep 1
echo "EAA222BB 16:24" >> $FIFO_CAMARAS
sleep 1
echo "EAA333CC 12:45" >> $FIFO_CAMARAS
sleep 1
echo "SAA333CC 22:04" >> $FIFO_CAMARAS
sleep 1
echo "SAA222BB 23:37" >> $FIFO_CAMARAS
sleep 1
echo "EASD123 15:13" >> $FIFO_CAMARAS
sleep 1
echo "SASD123 17:28" >> $FIFO_CAMARAS
sleep 1