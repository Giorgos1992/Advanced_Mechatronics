function plotFFT(Data) 
if mod(length(Data),2) == 1                   % Data should have an even number of samples
    Data = [Data 0];                % If not, pad with a zero
end
N = length(Data);
X = fft(Data);
mag(1) = abs(X(1))/N;               % DC component
mag(N/2+1) = abs(X(N/2+1))/N;       % Nyquist frequency component
mag(2:N/2) = 2*abs(X(2:N/2))/N;     % all other frequency components
freqs = linspace(0,1,N/2+1);        % x-axis a fraction of Nyquist freq
stem(freqs,mag);                    % plot the FFT magnitude plot
axis([-0.05 1.05 -0.1*max(mag) max(mag)+0.1*max(mag)]);
xlabel('Frequency (as fraction of Nyquist frequency)');
ylabel('Magnitude');
title('Single-Sided FFT Magnitude');
set(gca,'FontSize', 18);
end

