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
fclose(serial('COM6','Baudrate', 9600));
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
M2USB = serial('COM6','Baudrate', 9600);
% *** Use the device manager to check where the microcontroller is plugged
% into.

fopen(M2USB);       % Open up the port to the M2 microcontroller.
flushinput(M2USB);  % Remove anything extranneous that may be in the buffer.

% Send initial packet to get first set of data from microcontroller
fwrite(M2USB,1);% Send a packet to the M2.

%% Read in data and send confirmation packet
m2_buffer = fgetl(M2USB);   % Load buffer
fwrite(M2USB,1);

        [x, remain] = strtok(m2_buffer);
        [y, remain2] = strtok(remain);
        [a, remain3] = strtok(remain2);
        [x_top, remain4] = strtok(remain3);
        [y_top, remain5] = strtok(remain4);
        [x_bottom, remain6] = strtok(remain5);
        [y_bottom] = strtok(remain6);

        m2_buffer;


%figure;

%% Run program forever
try
    while 1
        
        %% Read in data and send confirmation packet
        m2_buffer = fgetl(M2USB);   % Load buffer
        fwrite(M2USB,1);            % Confirmation packet
        

        [x, remain] = strtok(m2_buffer);
        [y, remain2] = strtok(remain);
        [a, remain3] = strtok(remain2);
        [x_top, remain4] = strtok(remain3);
        [y_top, remain5] = strtok(remain4);
        [x_bottom, remain6] = strtok(remain5);
        [y_bottom] = strtok(remain6);
        m2_buffer;
        
        int_x = str2double(x);
       int_y = str2double(y);
       int_x_top = str2double(x_top);
       int_y_top = str2double(y_top);
       int_x_bottom = str2double(x_bottom);
       int_y_bottom = str2double(y_bottom);

        disp(m2_buffer);
        %% Plotting
        % figure(1);
        % clf;
        % hold on
        
       plot(int_x, int_y, '.', int_x_top,int_y_top, 'o', int_x_bottom,int_y_bottom,'o');

        axis([0 1023 0 768]);
        
        
        grid on
        pause(.04);
        
        hold off
        
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
