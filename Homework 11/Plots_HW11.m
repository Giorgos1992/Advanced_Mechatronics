clear all;
close all;
% hold on;
% hold all;
cd('C:\Users\Giorgos1992\Desktop\hidapi-master\hidapi-master\windows');
Data = importdata('accels.txt');

figure('Name', 'FFT of unfiltered raw data')
plotFFT(Data);

% The spike should appear around 0.5Hz (freq at which I moved the board)
% Sampling freq was 25Hz, Nyquist freq is 12.5Hz
% The spike should appear at 0.5/12.5 = around 0.04

%FIR

b=fir1(10,0.2);
figure('Name','FIR with 10 coef. and cut-off freq. at 0.2*Nyquist freq.')
freqz(b)
figure('Name','Unfiltered Data - Shaking platform')
plot(Data)
figure('Name','Filtered Data with FIR (10 coef.)')
plot(conv(b,Data))

abs(mean(Data))  % Should be the same as magnitude of spike
    
%MAF -- It helps reduce high frequency noise, by delaying signal 
 
maf = ones(10,1)/10; % No cut-off frequency requited; more coef., more delay, better filtering
figure('Name', 'MAF with 10 coef.')
freqz(maf)
figure('Name', 'Filtered Data with MAF (10 coef.)')
plot(conv(maf,Data));


Data2=conv(b,Data);
figure('NAME', 'FFT of filtered (FIR) Data')
plotFFT(Data2)
figure('NAME', 'FFT of filtered (MAF) Data')
Data3=conv(maf,Data);
plotFFT(Data3')