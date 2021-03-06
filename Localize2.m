clear all;
close all;

load('C.mat');

%initialize by grabbing first data point
x(1)=rawStarData(1,1);
x(2)=rawStarData(1,2);
x(3)=rawStarData(1,3);
x(4)=rawStarData(1,4);
y(1)=rawStarData(1,5);
y(2)=rawStarData(1,6);
y(3)=rawStarData(1,7);
y(4)=rawStarData(1,8);


n=1; %data point index
p=1; %plotting point index
distmax=0; %max distance to find furthest stars;
distmin=1023; %min distance to find closest stars;
dcenter=0; %distance to center point
rcenterx=512; %rink center in x
rcentery=384; %rink center in y

%indexes of max and min distance points
far1=0;
far2=0;
close1=0;
close2=0;

%indexes of stars
top=0;
bottom=0;
left=0;
right=0;

%angles for calculations if top and bottom points not present
beta=0;
theta=0;


while (distmax==0)
    %grab data points until all stars are seen(less than 1023)
    if ((x(1) < 1023) && (x(2)< 1023) && (x(3) < 1023) && (x(4) < 1023))
        
        %find max and min distance and store the indexes
        for i=1:4
            for j=(i+1):4
                dist=sqrt(((x(i)-x(j))^2)+((y(i)-y(j))^2));
                if (dist>distmax)
                    distmax=dist;
                    far1=i;
                    far2=j;
                    dcenter=dist/2;
                end
                if (dist<distmin)
                    distmin=dist;
                    close1=i;
                    close2=j;
                end
            end
        end
        n=n+1;
        
        %figure out which stars are which by comparing max and min distance
        %indexes
        if (far1==close1)
            top=far1;
            bottom=far2;
            right=close2;
            %left + top + bottom + right = 1 + 2 + 3 + 4 = 10
            left=10 - top - bottom - right;
        elseif (far1==close2)
            top=far1;
            bottom=far2;
            right=close1;
            left=10 - top - bottom - right;
        elseif (far2==close1)
            top=far2;
            bottom=far1;
            right=close2;
            left=10 - top - bottom - right;
        else
            top=far2;
            bottom=far1;
            right=close1;
            left=10 - top - bottom - right;
        end
    else
        n=n+1;
        x(1)=rawStarData(n,1);
        x(2)=rawStarData(n,2);
        x(3)=rawStarData(n,3);
        x(4)=rawStarData(n,4);
        y(1)=rawStarData(n,5);
        y(2)=rawStarData(n,6);
        y(3)=rawStarData(n,7);
        y(4)=rawStarData(n,8);
        
    end
end


for i=n:size(rawStarData)
    x(1)=rawStarData(n,1);
    x(2)=rawStarData(n,2);
    x(3)=rawStarData(n,3);
    x(4)=rawStarData(n,4);
    y(1)=rawStarData(n,5);
    y(2)=rawStarData(n,6);
    y(3)=rawStarData(n,7);
    y(4)=rawStarData(n,8);
    n=n+1;
    
    %recalculate indexes if more than 1 star is lost
    if(distmax==0)
        if ((x(1) < 1023) && (x(2)< 1023) && (x(3) < 1023) && (x(4) < 1023))
            
            %find max and min distance and store the indexes
            for i=1:4
                for j=(i+1):4
                    dist=sqrt(((x(i)-x(j))^2)+((y(i)-y(j))^2));
                    if (dist>distmax)
                        distmax=dist;
                        far1=i;
                        far2=j;
                        dcenter=dist/2;
                    end
                    if (dist<distmin)
                        distmin=dist;
                        close1=i;
                        close2=j;
                    end
                end
            end
            
            %figure out which stars are which by comparing max and min distance
            %indexes
            if (far1==close1)
                top=far1;
                bottom=far2;
                right=close2;
                %left + top + bottom + right = 1 + 2 + 3 + 4 = 10
                left=10 - top - bottom - right;
            elseif (far1==close2)
                top=far1;
                bottom=far2;
                right=close1;
                left=10 - top - bottom - right;
            elseif (far2==close1)
                top=far2;
                bottom=far1;
                right=close2;
                left=10 - top - bottom - right;
            else
                top=far2;
                bottom=far1;
                right=close1;
                left=10 - top - bottom - right;
            end
            
        else
            x(1)=1023;
            x(2)=1023;
            x(3)=1023;
            x(4)=1023;
        end
    end
    
    %cover all possible combinations of two stars to find position and
    %orientation
    if ((x(top) < 1023) && (x(bottom) < 1023))
        %calculate and store pixel-space position (midpoint of top and
        %bottom)
        posx(p)=(((x(top)+x(bottom))/2))-rcenterx;
        posy(p)=(((y(top)+y(bottom))/2))-rcentery;
        
        %calculate and store angle
        angle(p)=atan2(x(top)-x(bottom),y(top)-y(bottom));
        phi=-atan2(posy(p),posx(p));
        
        
        r=sqrt((posx(p)^2)+(posy(p)^2));
        
        realx(p)=rcenterx-r*cos(angle(p)-phi);
        realy(p)=rcentery+r*sin(angle(p)-phi);
       

        p=p+1;
     
    %use any combination of 2 points to find position and angle    
    elseif ((x(top) < 1023) && (x(left) < 1023))
        beta=atan2(x(top)-x(left),y(top)-y(left));
        theta=atan2(14.5-2.483,10.563);
        posx(p)=rcenterx-(x(top)-dcenter*sin(theta-beta));
        posy(p)=rcentery-(y(top)+0.5*dcenter*cos(theta-beta));
        
        angle(p)=theta+beta;
        phi=-atan2(posy(p),posx(p));
        
        r=sqrt((posx(p)^2)+(posy(p)^2));
        
        realx(p)=rcenterx-r*cos(angle(p)-phi);
        realy(p)=rcentery-r*sin(angle(p)-phi);

        p=p+1;
        
        disp('1');        
        
        
    elseif ((x(top) < 1023) && (x(right) < 1023))
        beta=atan2(x(top)-x(right),y(top)-y(right));
        theta=atan2(11.655,14.5+8.741);
        posx(p)=rcenterx-(x(top)+0.5*dcenter*sin(theta-beta));
        posy(p)=rcentery-(y(top)-0.5*dcenter*cos(theta-beta));
        
        angle(p)=-theta-beta;
        phi=-atan2(posy(p),posx(p));
        
        r=sqrt((posx(p)^2)+(posy(p)^2));
        
        realx(p)=rcenterx-r*cos(angle(p)-phi);
        realy(p)=rcentery-r*sin(angle(p)-phi);
        
        p=p+1;
        
        disp('2');
        
    elseif ((x(bottom) < 1023) && (x(left) < 1023))
        beta=atan2(x(bottom)-x(left),y(bottom)-y(left));
        theta=atan2(10.563,14.5+2.483);
        posx(p)=rcenterx-(x(bottom)+dcenter*sin(theta-beta));
        posy(p)=rcentery-(y(bottom)-dcenter*cos(theta-beta));
        
        angle(p)=(3.14/2)+theta-beta;
        phi=-atan2(posy(p),posx(p));
        
        r=sqrt((posx(p)^2)+(posy(p)^2));
        
        realx(p)=rcenterx-r*cos(angle(p)-phi);
        realy(p)=rcentery-r*sin(angle(p)-phi);
        
        p=p+1;
        
        disp('3');
        
    elseif ((x(bottom) < 1023) && (x(right) < 1023))
        beta=atan2(x(bottom)-x(right),y(bottom)-y(right));
        theta=atan2(11.655,14.5-8.741);
        posx(p)=rcenterx-(x(bottom)+0.5*dcenter*sin(theta-beta));
        posy(p)=rcentery-(y(bottom)-0.5*dcenter*cos(theta-beta));
        
        angle(p)=theta-beta;
        phi=-atan2(posy(p),posx(p));
        
        r=sqrt((posx(p)^2)+(posy(p)^2));
        
        realx(p)=rcenterx-r*cos(angle(p)-phi);
        realy(p)=rcentery-r*sin(angle(p)-phi);
        
        p=p+1;
        
        disp('4');        
        
    end
    
    %find out how many stars are left
    nostar=0;
    for i=1:4
        if (x(i) == 1023)
            nostar=nostar+1;
        end
    end
    
    %if more than one star is lost, recalibrate
    if (nostar > 1)
        distmax = 0;
    end
    
end

for i=1:length(realx)
    subplot(2,1,1)
    plot(realx(1:i),realy(1:i),'k.');
    title('Location of Robot');
    axis([0 1024 0 768]);
    xlabel('X-Position');
    ylabel('Y-position');

    subplot(2,1,2)
    plot(1:i,angle(1:i));
    title('Orientation of Robot');
    xlabel('Sample Number');
    ylabel('Angle (Radians)');
    axis([0 length(realx) -4 4]);
    pause(0.01);
    
end







    






    
    
    
    
    

