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
fclose(serial('COM4','Baudrate', 9600));
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
M2USB = serial('COM4','Baudrate', 9600);
% *** Use the device manager to check where the microcontroller is plugged
% into.

fopen(M2USB);       % Open up the port to the M2 microcontroller.
flushinput(M2USB);  % Remove anything extranneous that may be in the buffer.

% Send initial packet to get first set of data from microcontroller
fwrite(M2USB,1);% Send a packet to the M2.

%% Read in data and send confirmation packet
m2_buffer = fgetl(M2USB);   % Load buffer
fwrite(M2USB,1);

        [m2_x, remain] = strtok(m2_buffer);
        [m2_y, remain2] = strtok(remain);
        [m2_a, remain3] = strtok(remain2);
%         [m2_x_top, remain4] = strtok(remain3);
%         [m2_y_top, remain5] = strtok(remain4);
%         [m2_x_bottom, remain6] = strtok(remain5);
%         [m2_y_bottom, remain7] = strtok(remain6);
%         [m2_x_right, remain8] = strtok(remain7);
%         [m2_y_right, remain9] = strtok(remain8);
%         [m2_x_left, remain10] = strtok(remain9);
%         [m2_y_left, remain11] = strtok(remain10);
%         [m2_phi, remain12] = strtok(remain11);
%         [m2_posx, remain13] = strtok(remain12);
%         [m2_posy, remain14] = strtok(remain13);
%         [m2_r] = strtok(remain14);
        m2_buffer;

figure;


%% Run program forever
try
    while 1
        
        %% Read in data and send confirmation packet
        m2_buffer = fgetl(M2USB);   % Load buffer
        fwrite(M2USB,1);            % Confirmation packet
        
        [m2_x, remain] = strtok(m2_buffer);
        [m2_y, remain2] = strtok(remain);
        [m2_a, remain3] = strtok(remain2);
%         [m2_x_top, remain4] = strtok(remain3);
%         [m2_y_top, remain5] = strtok(remain4);
%         [m2_x_bottom, remain6] = strtok(remain5);
%         [m2_y_bottom, remain7] = strtok(remain6);
%         [m2_x_right, remain8] = strtok(remain7);
%         [m2_y_right, remain9] = strtok(remain8);
%         [m2_x_left, remain10] = strtok(remain9);
%         [m2_y_left, remain11] = strtok(remain10);
%         [m2_phi, remain12] = strtok(remain11);
%         [m2_posx, remain13] = strtok(remain12);
%         [m2_posy, remain14] = strtok(remain13);
%         [m2_r] = strtok(remain14);
        m2_buffer;
        
        Wii_m2_x = str2double(m2_x);
        Wii_m2_y = str2double(m2_y);
%         Wii_m2_x_top = str2double(m2_x_top);
%         Wii_m2_y_top = str2double(m2_y_top);
%         Wii_m2_x_bottom = str2double(m2_x_bottom);
%         Wii_m2_y_bottom = str2double(m2_y_bottom);
%         Wii_m2_x_right = str2double(m2_x_right);
%         Wii_m2_y_right = str2double(m2_y_right);
%         Wii_m2_x_left = str2double(m2_x_left);
%         Wii_m2_y_left = str2double(m2_y_left);
%         Wii_m2_posx = str2double(m2_posx);
%         Wii_m2_posy = str2double(m2_posy);
%         Wii_m2_r = str2double(m2_r);
        
        %% Plotting
        % figure(1);
        % clf;
        % hold on
        
        plot(Wii_m2_x, Wii_m2_y, '.');%, Wii_m2_x_top,Wii_m2_y_top,'om', Wii_m2_x_bottom,Wii_m2_y_bottom,'og', Wii_m2_x_right,Wii_m2_y_right,'+r', Wii_m2_x_left,Wii_m2_y_left,'+b', Wii_m2_posx, Wii_m2_posy, 'p');
        disp(m2_buffer);
        %disp(Wii_m2_r);
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
