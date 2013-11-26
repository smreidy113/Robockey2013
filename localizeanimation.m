clear all;


load('C.mat');

x1=128-(128/1023)*rawStarData(:,1);
x2=128-(128/1023)*rawStarData(:,2);
x3=128-(128/1023)*rawStarData(:,3);
x4=128-(128/1023)*rawStarData(:,4);
y1=96-(96/1023)*rawStarData(:,5);
y2=96-(96/1023)*rawStarData(:,6);
y3=96-(96/1023)*rawStarData(:,7);
y4=96-(96/1023)*rawStarData(:,8);

%just current point
for i=1:size(x1)
    plot(x1(i),y1(i),'k.',x2(i),y2(i),'g.',x3(i),y3(i),'r.',x4(i),y4(i),'b.');
    axis([0 128 0 96]);
    title('Location of Robot');
    xlabel('X-Position');
    ylabel('Y-position');
    pause(0.01);
end

%all points leading up to current point
for i=1:size(x1)
    plot(x1(1:i),y1(1:i),'k.',x2(1:i),y2(1:i),'g.',x3(1:i),y3(1:i),'r.',x4(1:i),y4(1:i),'b.');
    axis([0 128 0 96]);
    title('Location of Robot');
    xlabel('X-Position');
    ylabel('Y-position');
    pause(0.01);
end