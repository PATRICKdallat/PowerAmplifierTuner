# PowerAmplifierTuner

ABSTRACT 
Live Sound Engineers have the role of tuning the PA system (Power Amplification System) before an event and this role is time-consuming especially for less experienced Sound Engineers. This project aims to produce a tool for Sound Engineers to help guide/direct them to the problematic areas of the acoustic profile of the venue, and advise the Sound Engineer how to take care of the problematic frequencies. This project is built from the ground up using C++ for fast and optimized functions to produce a system that analyses audio using self-built FFT functions in real-time. The Power Amplification Tuner system is Apple Macintosh based that creates a preset file for a Behringer x32 Mixing Console, with the additional use of any external Audio Interfaces (must be mac compatible).


# Table of Contents: 
  Introduction
  1. Background 										
    1.1 Overview										
    1.2 Digital Signal Processing								
      1.2.1 Analog to Digital Conversion 						
      1.2.2 Digital to Analog Conversion 						
      1.2.3 Uniform Sampling							
      1.2.4 Buffer									
    1.3 Discrete Fourier Transforms							
      1.3.1 Fast Fourier Transforms							
      1.3.2 Euler’s Formula								
  2. Problem Specification									
	  2.1 Overview 										
	  2.2 Process and Methods								
	  2.3 Target Audience									
	  2.4 Other Applications								
  3. Proposed Solution and Justification of Development Model			
  	3.1 Overview										
    3.2 Hardware Components 								
      3.2.1 Audio Interface								
      3.2.2 RTA Microphone							
    3.3 Software Components 								
      3.3.1 Graphical User Interface						
      3.3.2 Audio Output								
      3.3.3 Audio Input								
      3.3.4 Fast Fourier Transform							
     3.3.5 Pink Noise Generator							
      3.3.6 Equalisation Gain Structure						
      3.3.7 Writing a Preset File							
      3.3.8 MIDI I/O									
      3.3.9 EQ Sculptor

Introduction
This project was designed to develop my understanding of C++, the usage of external frameworks and establishing an understanding of the kernel structure. This report outlines and details the background of Power Amplification for live events, the development of a Power Amplifier Tuner application, and the understanding that has been gained from the development. The report will describe the physical structure of the system, the data structure of the application, and explain the translation of the equations that calculate Fast Fourier Transforms and their implementation into C++.  

Power Amplification Tuner application is a tool to assist a Sound Engineer through the setting up of a Power Amplification System at a live event, whether it be a concert, church event, conference, etc. The tool helps find the problematic frequencies and enables the Sound Engineer to speed up his/her setup time. 


1. Background

    1.1 Overview
This chapter reports on the research and background to the acquisition of knowledge needed before moving into the development stages. The next few sections are the bases for a programmer looking to venture into the world of Digital Signal Processing, taking it from a basic understanding of DSP to FFT in real-time.  

    1.2 Digital Signal Processing
Digital Signal Processing, also known as DSP, is the processing, analyzation and modification of a signal (ie: audio, light, data, etc.) to enhance the signal’s performance (rate of reproduction, quality, etc.). DSP is comprised of applying pre-existing algorithms or self-made algorithms to convert analog signals to digital signal. This process is call ‘ADC – Analog to Digital Conversion’ when, in the digital signal state, the signal can then be manipulated/ modified for the intended purpose.

The definition of a signal is information (ie: data, events, audio, etc.) that serves a function when it is transmitted/received by a certain party. Audio is the vibration of the air particles/other substances that produce audible material, therefore Audio signal is the transmission of the vibrations from one party to another.  From this point forward any references to signal will mean Audio Signal.

    1.2.1 Analog to Digital Conversion 
Analog to Digital Conversion (ADC) is the conversion process that takes place during the intake of signal via an input device, where the device will convert analogue signals (continuous quantity) into digital signals (discrete time digital representation). Analog signal is received as a continuous sinusoidal waveform that computer systems are unable to read. 

The process begins with the input signal being compared to the computer system’s standard input for signal (generally +1 to -1), this will enable the system to determine if incoming signal is high or low data intake. The constant measure of the incoming signal’s amplitude (volume/magnitude of data) during the process allows the computer system to store a binary representation of sound waves.
 
    1.2.2 Digital to Analog Conversion 
Digital to Analog Conversion (DAC) is the opposite conversion process that takes place during the outgoing of signal from a computer system via an output device. The conversion is from binary data into audio signal samples for the reproduction/ production of analog waveforms.

The process takes the constructed fixed-point binary number and translates that into a definite voltage (power) variable, for example, and this allows the output device to use this variable to power a loud speaker to vibrate to produce sound.

    1.2.3 Uniform Sampling
To record/extract data from a signal at uniform time intervals, sampling happens during the ADC stage, and the opposite at DAC is called reconstruction. The sample rate of sampling will determine the quality of reproduction of the audio signal in the digital domain. Having low sample rates will produce a less accurate depiction of the analog signal and high sample rates will produce high accuracy signals.


The bit rate of a sample is another factor that is at play when the quality of audio signal is being converted, with a low bit rate of 8bit producing fragmented audio that loses out on certain frequencies, while having a bit rate of 16-bit (standard) up to 24-bit will produce more clarity and a fuller frequency spectral content. 
    1.2.4 Buffers
Samples are stored in containers called Buffers, this container will be passed to the algorithms for processing of the digital signal. A frame of a buffer can hold from 32 to 2048 samples and buffer sizes go up in doubles from the previous buffer size. 

Latency is the delay before a transaction of data begins following an instruction. Buffer size is one element that affects latency of signal to the computer system; having a small buffer size decreases latency, however this will also increase CPU usage. 


  1.3 Discrete Fourier Transform
The analysis of a sampled audio signal in the frequency domain can be completed with the use of a Discrete Fourier Transform (DFT). A DFT is a tool that converts time-domain signal into frequency-domain data. This tool will be important in locating problematic frequencies for equalisation of the mixing console and this process will be explained in the following chapters. 


    1.3.1 Fast Fourier Transform
Fast Fourier Transform (FFT) is a numerically efficient method of calculating Discrete Fourier Transforms. The method of FFT was invented by Carl Friedrich Gauss in 1805, but the FFT’s algorithm was further developed and re-invented by J. W. Cooley and J. W. Tukey in 1965. (Press, 2007, p608) This fast-computational algorithm takes an array of time-domain wave-form samples then processes them to produce an array of frequency-domain spectrum samples. The number of samples must be a power of two as this makes calculations easier and more efficient for the system. The number of samples will be the same in the time-domain as the frequency-domain. Time-domain samples will produce real-valued samples and the frequency-domain will produce complex-valued samples, the complex values can calculate the magnitude and phase of samples.

In Big O notation DFT is O(N2) and FFT is O(Nlog2N) which calculates multiple samples at a faster rate based on the divide and conquer algorithm. The divide and conquer algorithm splits the incoming signal into two, calculates the DFT and joins the signal into one large DFT signal. (Press, 2007, p609)  


    1.3.2 Euler’s Formula
Euler’s Formula, [eix = cos x + i sin x], calculates a complex-valued for a give sample. Real-valued sample is a value that reflect the amount of incoming signal, and an imaginary-value is a new division of numbers which were derived for use in signal processing (Boulanger, 2011, p867).

Imaginary-values consist of two real numbers with unique properties (cosx and sinx). Euler’s Formula helps construct the FFT algorithm to produce frequency spectrums.


# 2. Problem Specification

    2.1 Overview
During the setup of a Power Amplification (PA) (ie: loud speaker system) for an event, the Sound Engineer must tune the PA system.  The tuning of a PA system is the process of setting the equalisation gain structure for the main outputs to the loud speakers.  This process is carried out to reduce the chances of audio feedback between the loudspeakers and the microphone(s), and to maximize volume before audio feedback occurs. 


    2.2 Process and Methods
Surface materials, size of venue, height of ceiling and loud-speaker placement are some of the many different variables that pertain to the acoustic profile of the venue. Acoustics is the physical quality of an environment that causes sounds to reverberate. The acoustic profile is what triggers frequencies to resonant at lower gain levels than others, thus causing the PA system to feedback. 

The fundamental components of a PA system consist of loud-speakers, microphone(s), PA and a mixing console which is where the sound engineer induces the PA system to feedback and sets the gain structure. The Sound Engineer, by raising the gain on the master left and right fader to unity, and raising the gain controls on selected frequencies for the main output will provoke certain frequencies to feedback. Finding frequencies that feedback the Sound Engineer reduces the gain on the select frequency or band of frequencies. This process is repeated to sufficiently reduce the gain structure of the main output to maximise the volume of the system without compromising the quality of amplified audio signal.

The purpose of the product is to assist a Sound Engineer in the tuning of the PA System. It is a tool designed to setup the equalisation of the PA System and to allow the Sound Engineer to adjust and refine the tuning of the PA System. The product should be used as a tool to tune the PA System to a certain point and to give the Sound Engineer a good base to work from. Additionally, it can be used as a teaching tool to show beginners how to tune a PA System.  
    
    2.3 Target Audience
Sound Engineers have many roles throughout a single event; these roles range from loading gear into the venue, placement and construction of staging and equipment, testing of equipment, connecting each component of the PA system and instruments (microphones, musical instruments, line in (audio sources - laptops)), to line checks. All the previously stated jobs must be completed before the PA system is tuned. The Sound Engineer manages the quality of the audio and this application will allow Sound Engineers to focus their time on sound checking the artist, speaker, or production and mixing the best sound for the event that they possibly can.

    2.4 Other Applications
Market research into the process of tuning a PA system found that some Sound Engineers use Spectrum Analyser Applications to determine the problematic frequencies. The Spectrum Analyser Application allows the Sound Engineer to visualise the frequency spectrum, giving the Sound Engineer the ability to pinpoint frequencies that will cause problems when mixed with additional elements, eg: elements such as human voice, instruments, etc. 

The research also investigated hardware products in the market to help solve this problem and found that there are many spectrum analysis devices on the market, and only a few automated systems. The few automated systems on the market include rack units which are not cheap, such as the DBX DriveRack rack unit. DBX’s DriveRack is a product that is designed for larger scaled or funded productions and the ‘Power Amplifier Tuner’ application consists of equipment most Sound Engineers would have at their disposal, making this system more portable than other automated systems in the market. 



# 3. Proposed Solution and Justification of Development Model

    3.1 Overview
‘Power Amplifier Tuner’ is a system that aids Sound Engineers with the setup of a Power Amplification System; it will analyse the frequency spectrum profile of the incoming audio signal, display the frequency spectrum in real time and produce pink-noise out the system to allow a uniform analysis of the frequency spectrum. Breaking down each component, the next few sections will explore the components involved with the system. Here is an overview of the proposed solution’s hardware layout. The set of Loud-Speakers are connected to a mixing console of screen by two green cables [Fig.6 – A]; the mixing console will connect to the audio interface to a stereo input channel on the desk [Fig.6 – E]; the microphone will be connected straight to the audio interface’s input channel allowing audio input data to be recorded to the system [Fig.6 – D]; the audio interface is finally connected to the Macintosh Laptop to control the transmission and retrieval of audio data [Fig.6 – B]. 



    3.2 Hardware Components 
This system will comprise the setup of: a Macintosh computer; Macintosh compatible Audio Interface; microphone (preferably an RTA measurement microphone); connecting audio cables dependent on Audio Interface and the Power Amplification System.

    3.2.1 Audio Interface
Multiple Audio Interfaces can be used; one interface can be used to receive audio from microphone, one interface to output Pink Noise from the system and a third interface to control midi messages to the Audio Mixing console. Audio Interfaces can be selected for each function within the Graphical User Interface (GUI) setup and access to these input and output functions may be turned off and on via GUI setup. The main Audio Interface used for testing was the Focusrite Saffire Pro 24.

	
    3.2.2 RTA microphone
Real-Time Analysis (RTA) measurement microphone is unlike other microphones as it is designed to measure frequency spectrum with a flat frequency response. A flat frequency response means that the incoming frequency has not been coloured by the microphone, for example, some microphones are designed to boost the mid-frequencies to emphasize the frequency range that is normally occupied by the human voice - this is to help bring out certain qualities of vocals that are desirable in a performance. 

The measurement microphone’s use in this system is to pick up the venue’s frequency profile, in other words, to analyse the venue to locate the problematic frequencies. The microphone will be placed at the same distance from each loud speaker as each speaker is from each other, in an equilateral triangle.


    3.3 Software Components 
Software components for this system will be a mixer of single functions and entire classes. The functions span from sampling and construction of samples for audio in and out, FFT algorithms, GUI and Gain Structuring. With all these components, this system could take the form of a physical application for the use of Sound Engineers. Below will outline the roles of the different software elements.
 

    3.3.1 Graphical User Interface
GUI is a simple, intuitive and straight to the point graphical interface, where the user does not have to click, move the mouse or scroll through menus to achieve functionality. The colour scheme for the application is dark with contrasting colour focal points, i.e. frequency spectrum and equalisation curve. 

    3.3.2 Audio Output
The audio output component of the system was created to transmit generated Pink Noise data signal to the selected output audio system, as it reconstructs samples through the DAC process. 

    3.3.3 Audio Input
The audio input component of the system was created to receive sampled audio signal from the input audio interface, through the ADC process. The function will direct the signal data to the different functions and memory that need access to the information. The received signal will be audio from RTA measurement microphone, with the audio signal being converted and passed to FFT functions.

    3.3.4 Fast Fourier Transform
This component was created to verify that the input and output parameters are correct and to calculate the Fourier Transform in an efficient manner. This algorithm demands a lot of memory and, thus, memory management was key to receiving results fast and efficiently. 

    3.3.5 Pink Noise Generator
Pink Noise is static white noise but, unlike white noise, the random generation of signal has equal energy per octave, thus having more low-frequency content present in its signal than white noise.  Pink Noise audio is going to be directed through the Audio Output and the RTA measurement microphone will be there to pick up the frequency of the room by analysing the Pink Noise sounding through the speakers.


    3.3.6 Equalisation Gain Structure
The Pink Noise from the Audio Input will be analysed by retrieving the array of values created from the Fast Fourier Transform, and detecting the frequencies that are increasing in amplitude, or are too high in amplitude. After detecting problematic frequency, the function will set suitable gain structure for said frequency and stores the gain structure in an array.

    3.3.7 Writing a Preset File
Following the system setting gain structures, the system retrieves the array of gain structures and will create txt file that will write the gain structures as a preset that can be loaded for the Behringer x32 console. If the Sound Engineer is not using a Behringer x32 console he/she can manually structure the equalisation for the Left and Right main channels of the Mixing Console.

    3.3.8 MIDI I/O
The MIDI (Musical Instrument Device Interface) functions have been created for further development to allow direct access to a Mixing Console’s controls to allow full automations of the Power Amplification Tuning process. These functions will remain idle until the development of a function that produces MIDI event designed for the MIDI protocols of the Behringer x32 and other MIDI Mixing Consoles.

	  3.3.9 EQ Sculptor
The EQ Sculptor is the function that develops the equalisation gain structure for the given captured acoustic profile from the power spectrum transform. The 6 equalisation parameters are broken into frequency bands and each band has 3 levels of attenuation for the given frequency within the band. The EQ Sculptor function composes parameter data for the creation of preset files and develops parameters for the Equalisation Curve display. 
