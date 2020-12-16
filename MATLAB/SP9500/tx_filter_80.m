clc
close all
clear all

N = 41;
Fs = 245.76e6;
F_intvl = 2e6;
F_star = 80e6 / 2 + 1.5 * F_intvl;          %43MHz,3MHz是为了减轻吉布斯效应
F_stop = F_star + round((1 + 1) / N * Fs);  %Bt>=2pi / N * (m + 1),过渡带暂时取m=1个点

F = [-F_stop,-F_star : F_intvl : F_star,F_stop] / (Fs / 2);
F = [-1,F,1];

fid_real = fopen('c:\\CSECal\\tx_filter_80_coef_real.txt','w');
fid_imag = fopen('c:\\CSECal\\tx_filter_80_coef_imag.txt','w');

fid = fopen('c:\\CSECal\\tx_filter_80.txt','r');
tline = fgetl(fid);
while ischar(tline)
    Rsp = str2num(tline);
    Rsp = power(10.0,Rsp / 20.0);
    Rsp = Rsp./Rsp(23);
    
    A = [0,0,1./Rsp,0,0];
    W = [1,1.0 * ones(1,length(Rsp)/2),1];
    h = cfirpm(N-1,F,A,W);
    %
    %    plot(F,20 * log10([inf,inf,Rsp,inf,inf]));
    %    fvtool(h);
    %    H = fftshift(fft(h,Fs/(F_intvl / 2)));
    %    H = H(2:2:end);
    %    t_r = length(Rsp);
    %    t_H2 = length(H) / 2;
    %    H(t_H2 - t_r/2 + 1 : t_H2 + t_r/2) = H(t_H2 - t_r/2 + 1 : t_H2 + t_r/2).*Rsp;
    %    plot(20 * log10(abs(H)));
    %
    h_real = real(h);
    h_imag = imag(h);
    h_mod_max = 0.0;
     
    for m = 1:N
        h_mod = h_real(m) * h_real(m) + h_imag(m) * h_imag(m);
        if (h_mod > h_mod_max)
            h_mod_max = h_mod;
        end
    end
    h_mod_max = sqrt(h_mod_max);
     
    b = 15;
    quant = 2^(b - 1) - 1;
     
    for m = 1:N
        h_real(m) = h_real(m) / h_mod_max * quant;
        h_imag(m) = h_imag(m) / h_mod_max * quant;
    end
     
    fprintf(fid_real,'%d,',round(h_real));
    fprintf(fid_real,'\n');
    fprintf(fid_imag,'%d,',round(h_imag));
    fprintf(fid_imag,'\n');

    tline = fgetl(fid);
end

fclose(fid);
fclose(fid_imag);
fclose(fid_real);