%% ADC value liveplotting & logging.
%
% Reads ADC values from the M2 microcontroller.
% Plots the data realtime in a subplot.
% Logged into ADC_Log[time, ADC_Value] variable.
% Hit Ctrl-C to quit the program
%
% By Nick McGill
% Modified by Elizabeth Beattie in Fall 2012 for Acrobat IMU
% Modified again by Michael Lautman in March 2013 for teh Lulz.
% Reads m_imu values from the M2 microcontroller and plots the desired data
% in real time.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%% Initialize program and USB port
% Close any existing open port connections
% For the first time running, comment this.
% if(exist('M2USB'))
%     fclose(M2USB);
% else
%     fclose(instrfindall);
% end
%
% clear all
% close all
%

%% If the above initialization does not work, please run the following commands manually and disconnect and reconnect USB.
fclose(serial('COM7','Baudrate', 9600));
fclose(instrfindall);
clear all;
close all;

%VARIABLES
%maxPoints = 20; %display only 20 points at a time
%Wii_m2_x = zeros(1, maxPoints);
%Wii_m2_y = zeros(1, maxPoints);
%Wii_m2_a = zeros(1, maxPoints);

%% SERIAL
%----> for ***WINDOZE***
M2USB = serial('COM7','Baudrate', 9600);
% *** Use the device manager to check where the microcontroller is plugged
% into.

fopen(M2USB);       % Open up the port to the M2 microcontroller.
flushinput(M2USB);  % Remove anything extranneous that may be in the buffer.

% Send initial packet to get first set of data from microcontroller
fwrite(M2USB,1);% Send a packet to the M2.

%% Read in data and send confirmation packet
m2_buffer = fgetl(M2USB);   % Load buffer
fwrite(M2USB,1);

        [ADC1, remain] = strtok(m2_buffer);
        [ADC2, remain2] = strtok(remain);
        [ADC3, remain3] = strtok(remain2);
        [ADC4, remain4] = strtok(remain3);
        [ADC5, remain5] = strtok(remain4);
        [ADC6, remain6] = strtok(remain5);
        [ADC7] = strtok(remain6);
        m2_buffer;




%% Run program forever
try
    while 1
        
        %% Read in data and send confirmation packet
        m2_buffer = fgetl(M2USB);   % Load buffer
        fwrite(M2USB,1);            % Confirmation packet
        

        [ADC1, remain] = strtok(m2_buffer);
        [ADC2, remain2] = strtok(remain);
        [ADC3, remain3] = strtok(remain2);
        [ADC4, remain4] = strtok(remain3);
        [ADC5, remain5] = strtok(remain4);
        [ADC6, remain6] = strtok(remain5);
        [ADC7] = strtok(remain6);
        m2_buffer;
        
        int_ADC1 = str2double(ADC1);
        int_ADC2 = str2double(ADC2);
        int_ADC3 = str2double(ADC3);
        int_ADC4 = str2double(ADC4);
        int_ADC5 = str2double(ADC5);
        int_ADC6 = str2double(ADC6);
        int_ADC7 = str2double(ADC7);
        
        %% Plotting
        % figure(1);
        % clf;
        % hold on
        
        disp(m2_buffer);
        

        i=i+1;  % Incrememnt indexer
        %% Logging
        %    if(rem(i,LOGFREQUENCY) == 0)
        %        IMU_Log = [IMU_Log; time Angle_Actual];
        %    end
        
    end
    
catch ME
    ME.stack
    %Close serial object
    fclose(M2USB);
end
