%% Point Cloud Generation
% Sombrero function
% Description: creates a point cloud and applies 
% a random transformation, possibly adding noise and outliers. 
% Author: Pasquale Antonante
% Date: 
% MIT Copyright (c) Pasquale Antonante

clear all
close all
clc

addpath('./lib')

f = @(X,Y) (10*sin(sqrt(X.^2 + Y.^2)) + 0.1) ./ sqrt(X.^2 + Y.^2);
x_space = -8:.25:8;
y_space = -8:.25:8;
sigma = 0.05; % noise
beta = 3/100; % outliers percentage

%% Generation
ptCloud_Q = PCbyFunc(f,x_space,y_space);

%% apply random tranformation, add noise and outlier and save results
[ptCloud_P,T] = randomlyTransformPtCloud(ptCloud_Q,sigma,beta);

%% Save
pcwrite(ptCloud_Q,'ptCloud_Q.pcd','Encoding','ascii');
pcwrite(ptCloud_P,'ptCloud_P.pcd','Encoding','ascii');
SaveTransformationMatrix(T,'trans.mat');