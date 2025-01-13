% 读取第一个 Excel 文件
filename2 = '加热散热过程2.xls';  % 替换为你的文件名
data2 = readmatrix(filename2);
c=50;
d=length(data2);
time2 = data2(c:d, 1);  % 第一列是时间
temp2 = data2(c:d, 2);  % 第二列是温度
time2=time2;
% 确保两个时间向量相同（假设时间相同）
temp2_normalized = (temp2 - min(temp2)) / (max(temp2) - min(temp2));


% 绘制第一个图：两个温度的并列图
figure;
plot(time2, temp2_normalized', 'LineWidth', 2);  % 红色线表示第二个温度
xlabel('Time/s');
ylabel('TCA Length');
title('Normalized length of TCA under different conditions');
grid on;
set(gca,'FontName','Times New Roman','FontSize',24)
box off     % 取消边框
%axis([0 200 -40 40])
%xticks(0:50:200);
%yticks(-40:20:40);
ax1 = axes('Position',get(gca,'Position'),'XAxisLocation','top',...
    'YAxisLocation','right','Color','none','XColor','k','YColor','k');  % 设置坐标区
set(ax1,'XTick', [],'YTick', []);   % 去掉xy轴刻度

