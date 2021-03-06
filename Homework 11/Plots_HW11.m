clear all;
close all;
% hold on;
% hold all;
cd('C:\Users\Giorgos1992\Desktop\hidapi-master\hidapi-master\windows');
Data = importdata('accels.txt');

% Plot raw data
figure('Name','Raw Data - Shaking platform')
subplot(3,1,1)
plot(Data)
title('Raw data', 'FontSize', 14);
subplot(3,1,2)
plotFFT(Data);
subplot(3,1,3)
plotFFT(Data);


abs(mean(Data))  % Should be the same as magnitude of spike
% The spike should appear around 0.5Hz (freq at which I moved the board)
% Sampling freq was 25Hz, Nyquist freq is 12.5Hz
% The spike should appear at 0.5/12.5 = around 0.04

%MAF filtering -- It helps reduce high frequency noise, by delaying signal 
maf = ones(1,10)/10; % No cut-off frequency requited; more coef., more delay, better filtering
MAF_Data=conv(maf,Data); %Filters data with MAF coefficients

% MAF Filtering Plots
figure('Name','MAF filtering')
% figure('Name', 'MAF with 10 coef.')
freqz(maf)
h = get(gcf, 'children');   %gcf takes current figure- 'children' takes subexpressions/subplots
fig2=figure;
figure(fig2)
g=subplot(2,2,1)
set(h(1), 'pos', get(g, 'pos'))
figure(fig2)
g=subplot(2,2,3)
set(h(2), 'pos', get(g, 'pos'))
close
title(['Frequency response'])
subplot(2,2,2)
plot(MAF_Data)
title('Filtered Data with 10 coef.')
subplot(2,2,4)
plotFFT(MAF_Data')

%FIR filtering 
fir=fir1(10,0.2);
FIR_Data=conv(fir,Data); % Filter data with FIR coefficients

%FIR Filtering Plots
figure('Name','FIR filtering')
% figure('Name', 'MAF with 10 coef.')
freqz(fir)
h = get(gcf, 'children');   %gcf takes current figure- 'children' takes subexpressions/subplots
fig2=figure;
figure(fig2)
g=subplot(2,2,1)
set(h(1), 'pos', get(g, 'pos'))
figure(fig2)
g=subplot(2,2,3)
set(h(2), 'pos', get(g, 'pos'))
close
title(['Frequency response'])
subplot(2,2,2)
plot(FIR_Data)
title('Filtered Data with 10 coef.')
subplot(2,2,4)
plotFFT(FIR_Data)


% figure('Name','FIR with 10 coef. and cut-off freq. at 0.2*Nyquist freq.')
% freqz(b)
% figure('Name','Filtered Data with FIR (10 coef.)')
% plot(conv(b,Data))
% 
%     
% Data2=conv(b,Data);
% figure('NAME', 'FFT of filtered (FIR) Data')
% plotFFT(Data2)
% figure('NAME', 'FFT of filtered (MAF) Data')
% Data3=conv(maf,Data);
% plotFFT(Data3')

%FIR filtering plots

% 
% figure('NAME', 'FFT of filtered Data')
% subplot(2,1,1)
% plotFFT(Data2)
% title('FIR filter', 'FontSize', 14);
% subplot(2,2,3)
% plotFFT(Data3')
% title('MAF filter', 'FontSize', 14, 'Color','red');
% subplot(2,2,4)
% plotFFT(Data2)
% title('FIR filter', 'FontSize', 14);

