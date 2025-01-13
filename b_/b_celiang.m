g1=readmatrix('b_damping.xlsx');
g1=g1(1:200,:); %You need to pick the right time to start and finis
timedata=g1(1:height(g1),1);%
ydata=g1(1:height(g1),2);
%timedata=table2array(timedata);
%ydata=table2array(ydata);
timedata2=zeros(height(g1),1);
b1=[ 0 0 0 0 ];
for i=1:height(g1)
    timedata2(i)=timedata(i)-timedata(1);
end
timedata2=timedata2;
b0=[ 0.0008651 0.015519  193 -48];%%初始值
f=@(b,x) b(4).*exp(-b(1).*x).*cos(b(2)*x)+b(3);
mdl=fitnlm(timedata2,ydata,f,b0);
b1(1)=table2array(mdl.Coefficients(1,"Estimate"));
b1(2)=table2array(mdl.Coefficients(2,"Estimate"));
b1(3)=table2array(mdl.Coefficients(3,"Estimate"));
b1(4)=table2array(mdl.Coefficients(4,"Estimate"));
plot(timedata2/1000,ydata-160,'r','LineWidth',2)
hold on
plot(timedata2/1000,b1(4).*exp(-b1(1).*timedata2).*cos( b1(2)*timedata2)+b1(3)-160,'b','LineWidth',1)
legend('Date', 'Fit');
xlabel('Time/s' ,'FontSize', 28)
ylabel('Displacement/mm' ,'FontSize', 28);
set(gca,'FontName','Times New Roman','FontSize',24)
box off     % 取消边框
%axis([0 200 -40 40])
%xticks(0:50:200);
%yticks(-40:20:40);
ax1 = axes('Position',get(gca,'Position'),'XAxisLocation','top',...
    'YAxisLocation','right','Color','none','XColor','k','YColor','k');  % 设置坐标区
set(ax1,'XTick', [],'YTick', []);   % 去掉xy轴刻度
