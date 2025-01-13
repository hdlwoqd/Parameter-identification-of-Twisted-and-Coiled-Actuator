% 读取数据
data = readmatrix('温度力表格.xlsx');

% 提取横坐标和数据
x3 = data(:, 3);  % 横坐标3
y = data(:, 4)*9.8;   % 数据

% 找到分割点
split_indices = find(x3 == 0 | y == 0);

% 初始化分割后的数据
x3_split = cell(1, length(split_indices) + 1);
y_split = cell(1, length(split_indices) + 1);

% 分割数据
start_idx = 1;
for i = 1:length(split_indices)
    end_idx = split_indices(i) - 1;
    x3_split{i} = x3(start_idx:end_idx);
    y_split{i} = y(start_idx:end_idx);
    start_idx = split_indices(i) + 1;
end
% 处理最后一组数据
x3_split{end} = x3(start_idx:end);
y_split{end} = y(start_idx:end);

% 绘制散点图和线性回归拟合线
figure;
hold on;
colors = lines(length(x3_split));  % 获取颜色

% 初始化存储拟合参数和斜率置信区间的数组
fit_params = cell(1, length(x3_split));
slope_ci = cell(1, length(x3_split));

for i = 1:length(x3_split)
    % 去除NaN值
    valid_indices = ~isnan(x3_split{i}) & ~isnan(y_split{i});
    x3_valid = x3_split{i}(valid_indices);
    y_valid = y_split{i}(valid_indices);
    
    % 线性回归
    p = polyfit(x3_valid, y_valid, 1);
    y_fit = polyval(p, x3_valid);
    
    % 计算残差标准差
    sigma = sqrt(sum((y_valid - y_fit).^2) / (length(y_valid) - 2));
    
    % 计算斜率的标准误差
    x_mean = mean(x3_valid);
    se_m = sigma / sqrt(sum((x3_valid - x_mean).^2));
    
    % 计算斜率的95%置信区间
    t_value = tinv(0.975, length(y_valid) - 2);  % t分布的97.5%分位数
    m_ci = [p(1) - t_value * se_m, p(1) + t_value * se_m];
    
    % 存储拟合参数和斜率置信区间
    fit_params{i} = p;
    slope_ci{i} = m_ci;
    
    % 绘制散点图和拟合线
    scatter(x3_valid, y_valid, [], colors(i, :), 'filled');
    plot(x3_valid, y_fit, 'Color', colors(i, :), 'LineWidth', 2);
end

% 设置图形属性
xlabel('Temperature(℃)');
ylabel('Force(N)');
legend('Data1', 'Fit1', 'Data2', 'Fit2', 'Data3', 'Fit3', 'Data4', 'Fit4', 'Location', 'best');
grid on;
hold off;
set(gca,'FontName','Times New Roman','FontSize',24)
box off     % 取消边框
%axis([0 200 -40 40])
%xticks(0:50:200);
%yticks(-40:20:40);
ax1 = axes('Position',get(gca,'Position'),'XAxisLocation','top',...
    'YAxisLocation','right','Color','none','XColor','k','YColor','k');  % 设置坐标区
set(ax1,'XTick', [],'YTick', []);   % 去掉xy轴刻度

% 输出拟合参数和斜率的95%置信区间
for i = 1:length(fit_params)
    fprintf('拟合参数 (斜率, 截距) 第 %d 组: [%.4f, %.4f]\n', i, fit_params{i}(1), fit_params{i}(2));
    fprintf('斜率的95%%置信区间 第 %d 组: [%.4f, %.4f]\n', i, slope_ci{i}(1), slope_ci{i}(2));
end