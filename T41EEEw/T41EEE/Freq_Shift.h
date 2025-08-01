#pragma once

#include "SDT.h"

/*****
  Purpose: void FreqShift1()
          AFP 12-31-20
        Frequency translation by Fs/4 without multiplication from Lyons (2011):
chapter 13.1.2 page 646 together with the savings of not having to shift/rotate
the FFT_buffer, this saves about 1% of processor use

        This is for +Fs/4 [moves receive frequency to the left in the spectrum
display] float_buffer_L contains I = real values float_buffer_R contains Q =
imaginary values xnew(0) =  xreal(0) + jximag(0) leave first value (DC
component) as it is! xnew(1) =  - ximag(1) + jxreal(1) Parameter list: void

  Return value:
    void
*****/
void FreqShift1();

/*****
  Purpose: Shift Receive frequency by an arbitrary amount

  Parameter list:
    void

  Return value;
    void
    Notes:  Routine includes checks to ensure the frequency selection stays
within the bounds of the displayed spectrum Also included a variable frequency
step, depending on how fast the encoder id turned.  Step varies from 50Hz/step
to 10KHz/step

    freq_conv2()

    FREQUENCY CONVERSION USING A SOFTWARE QUADRATURE OSCILLATOR (NCO)

    THIS VERSION calculates the COS AND SIN WAVE on the fly AND IS SLOW

    MAJOR ADVANTAGE: frequency conversion can be done for any frequency !

    large parts of the code taken from the mcHF code by Clint, KA7OEI, thank
you! see here for more info on quadrature oscillators: Wheatley, M. (2011):
CuteSDR Technical Manual Ver. 1.01. - http://sourceforge.net/projects/cutesdr/
    Lyons, R.G. (2011): Understanding Digital Processing. – Pearson, 3rd
edition. Requires 4 complex multiplies and two adds per data point within the
time domain buffer.  Applied after the data stream is sent to the Zoom FFT , but
befor decimation.
*****/
void FreqShift2();
