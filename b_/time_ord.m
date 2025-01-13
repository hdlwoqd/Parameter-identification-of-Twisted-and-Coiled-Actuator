% 读取 Excel 文件
filename = '实验Cth序号2第二次实验.xls';
g = readtable(filename);

% 选择数据点为开始后9个数据点到停止
timedata = g{100:height(g), 1};  % 选择第一列作为时间数据
ydata = g{100:height(g), 2};     % 选择第二列作为温度数据

% 数据预处理
timedata2 = zeros(length(ydata), 1);
b1 = [0 0 0 0];
for i = 1:length(ydata)
    timedata2(i) = (timedata(i) - timedata(1));  % 计算新的时间数据
end

% 初始值
b0 = [0.000085 80 50];

% 定义拟合函数
f = @(b, x) b(3) .* exp(-b(1) .* x) + b(2);

% 非线性拟合
mdl = fitnlm(timedata2, ydata, f, b0);

% 提取拟合参数
b1(1) = mdl.Coefficients{1, "Estimate"};
b1(2) = mdl.Coefficients{2, "Estimate"};
b1(3) = mdl.Coefficients{3, "Estimate"};

% 绘制原始数据和拟合曲线
figure(1)
plot(timedata2/1000, ydata,  'LineWidth', 4)
hold on
plot(timedata2/1000, b1(3) .* exp(-b1(1) .* timedata2) + b1(2), 'LineWidth', 3)
xlabel('Time/s')
ylabel('Temperature/℃') 

legend('Data', 'Fit','Location', 'best');
set(gca,'FontName','Times New Roman','FontSize',24)
box off     % 取消边框
%axis([0 200 -40 40])
%xticks(1.3:0.5:5.3);
%yticks(-40:20:40);
ax1 = axes('Position',get(gca,'Position'),'XAxisLocation','top',...
    'YAxisLocation','right','Color','none','XColor','k','YColor','k');  % 设置坐标区
set(ax1,'XTick', [],'YTick', []);   % 去掉xy轴刻度

% 计算时间常数
Time = 1/b1(1)/1000

% 绘制残差图
figure(2)
plot(timedata2, ydata - b1(3) .* exp(-b1(1) .* timedata2) - b1(2))
xlabel('时间/s')
ylabel('残差/℃')
title('残差图')