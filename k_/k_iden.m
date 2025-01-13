% 读取xlsx文件的前两列
data = readmatrix('应变-载荷.xlsx');

% 提取横坐标和数据
x = data(:, 1);
y = data(:, 2)*100;

% 使用unique函数获取唯一的横坐标，并计算每个横坐标对应的最大值
[unique_x, ~, idx] = unique(x);
max_y = accumarray(idx, y, [], @max);

% 绘制折线图
figure;
plot(unique_x, max_y, '-o',LineWidth=2);

% 设置图形属性
xlabel('Load/N');
ylabel('Contraction strain/% ');
title('The contraction strain of TCA under different loads');
grid on;

for i = 1:length(unique_x)
    text(unique_x(i), max_y(i)+1, sprintf('%.2f', max_y(i)), 'VerticalAlignment', 'bottom', 'HorizontalAlignment', 'right');
end
