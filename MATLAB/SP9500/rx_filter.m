clc
close all
clear all

N = 96;
i_Factor = 2; % 2 interpolation
d_Factor = 3; % 3 decimation
F_intvl = 2e6;
Fs = 368.64e6;
b = 12; % quantization
quant = 2^(b - 1) - 1;

% BW = 160MHz
F_star = 160e6 / 2 + 1.5 * F_intvl; % 3MHz to optimize Gibbs effect
F_stop = 92e6;                      % DDC = -92.64MHz
F = [-F_stop,-F_star : F_intvl : F_star,F_stop] / (Fs / 2);
F = [-1,F,1];

% ((160 + 3 + 3) - 100) / 2 / 2MHz Step / 2 = 8.25 ~= 8
% (160 + 3 + 3) / 2 / 2MHz Step / 2 - 8 ~= 13
W1 = 1 * ones(1,8);
W2 = 10 * ones(1,13);
W3 = 10 * ones(1,13);
W4 = 1 * ones(1,8);
W = [0.1,W1,W2,W3,W4,0.1];
    
fid_real = fopen('C:\\CSECal\\rx_filter_160_coef_real.txt','w');
fid_imag = fopen('C:\\CSECal\\rx_filter_160_coef_imag.txt','w');
fid = fopen('C:\\CSECal\\rx_filter_160.txt','r');

tline = fgetl(fid);
while ischar(tline)
    Rsp = str2num(tline);
    Rsp = power(10.0,Rsp / 20.0);
    Rsp = Rsp./Rsp(43);
    
    A = [0,0,1./Rsp,0,0];
    h = cfirpm(N-1,F,A,W);
    
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

% BW = 80MHz
F_star = 80e6 / 2 + 1.5 * F_intvl; % 3MHz to optimize Gibbs effect
F_stop = F_star + round((1 + 1) / N * Fs); % Bt >= 2pi / N * (m + 1),transition m = 1
F = [-F_stop,-F_star : F_intvl : F_star,F_stop] / (Fs / 2);
F = [-1,F,1];
b = 11; % quantization
quant = 2^(b - 1) - 1;

fid_real = fopen('C:\\CSECal\\rx_filter_80_coef_real.txt','w');
fid_imag = fopen('C:\\CSECal\\rx_filter_80_coef_imag.txt','w');
fid = fopen('C:\\CSECal\\rx_filter_80.txt','r');

tline = fgetl(fid);
while ischar(tline)
    Rsp = str2num(tline);
    Rsp = power(10.0,Rsp / 20.0);
    Rsp = Rsp./Rsp(23);
    
    A = [0,0,1./Rsp,0,0];
    W = [0.1,1 * ones(1,length(Rsp)/2),0.1];
    h = cfirpm(N-1,F,A,W);
    
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