function ex_loadsave
% This example shows how to work with the "load/save" functions
% on data files in deployed mode. There are three source data files
% in this example.
%    user_data.mat 
%    userdata/extra_data.mat 
%    ../externdata/extern_data.mat
%
% Compile this example with mcc command: 
%     mcc -m ex_loadsave.m -a 'user_data.mat'
%         -a './userdata/extra_data.mat'
%         -a '../externdata/extern_data.mat'
%
% User data files are unchanged by mcc. There is no excryption of user included
% data files. All '-a' data files will be included in the CTF archive and
% expanded under $ctfroot.
% Note: the MATLAB function 'ctfroot' will return the value $ctfroot at runtime.
%
% Any '-a' file located in the main '-m' file's directory at compile time,
% will be located in the directory $ctfroot\$mfilename at run time, where
% $mfilename is the name of the main '-m' MATLAB file, without the '.m'.
% In this example, the run time location of 'user_data.mat' will be:
%	'$ctfroot\ex_loadsave\user_data.mat',
% This directory will be on the MATLAB runtime path of the compiled application.
% Note: The MATLAB function 'mfilename' will return $mfilename for the currently
% executing MATLAB file.
%
% Any '-a' file located in a folder under the main '-m' file's directory
% at compile time, will be located at the same path relative to $ctfroot at
% run time. In this example, the run time location of 'extra_data.mat' will be:
%	'$ctfroot\userdata\extra_data.mat'.
% This directory will be on the MATLAB runtime path of the compiled application.
%
% Any '-a' file NOT located in or under the main '-m' file's directory at
% compile time, will have its absolute path from the disk drive or filesystem
% root appended to $ctfroot to give its run time location.
% In this example, since the compile time location of 'extern_data.mat' is:
%	'c:\$matlabroot\examples\externdata\extern_data.mat',
% its run time location will be:
%	'$ctfroot\$matlabroot\examples\externdata\extern_data.mat'.
% This directory will be on the MATLAB runtime path of the compiled application.
% Note: $matlabroot here is the MATLAB installation root of the compile time 
% environment, NOT what the MATLAB function 'matlabroot' will return at runtime.
%
%  In this example, the output data file is written to the path:
%	output/saved_data.mat
% relative to the application's run time current working directory.
% When writing data files to local disk, do not save any files under $ctfroot,
% as $ctfroot may be deleted and/or refreshed for each application run.
%
%==== load data file =============================
if isdeployed
    % In deployed mode, all files in or under the main file's directory
    % may be loaded by full path, by path relative to ctfroot, or by
    % filename only, since their directories are on the MATLAB path.
    % Here we load 'user_data.mat' by full path.
    LOADFILENAME1=which(fullfile(ctfroot,mfilename,'user_data.mat'));    
    % These alternate methods would also work:
    % LOADFILENAME1=which(fullfile(mfilename,'user_data.mat'));    
    % LOADFILENAME1=which(fullfile('user_data.mat'));

    % Here we load 'extra_data.mat' by relative path:
    LOADFILENAME2=which(fullfile('userdata','extra_data.mat'));
    % These alternate methods would also work:
    % LOADFILENAME2=which(fullfile(ctfroot,'userdata','extra_data.mat'));
    % LOADFILENAME2=which(fullfile('extra_data.mat'));

    % For a data file external to the main MATLAB file's directory tree,
    % its full compile time path has been appended to $ctfroot, so it is
    % best to simply load it by filename (since it is on the path):
    LOADFILENAME3=which(fullfile('extern_data.mat'));
else
    %running the code in MATLAB
    LOADFILENAME1=fullfile(matlabroot,'extern','examples','compiler','Data_Handling','user_data.mat');
    LOADFILENAME2=fullfile(matlabroot,'extern','examples','compiler','Data_Handling','userdata','extra_data.mat');
    LOADFILENAME3=fullfile(matlabroot,'extern','examples','compiler','externdata','extern_data.mat');
end

% Load the data file from current working directory
disp(['Load A from : ',LOADFILENAME1]);
load(LOADFILENAME1,'data1');
disp('A= ');
disp(data1);

% Load the data file from sub directory
disp(['Load B from : ',LOADFILENAME2]);
load(LOADFILENAME2,'data2');
disp('B= ');
disp(data2);

% Load extern data outside of current working directory
disp(['Load extern data from : ',LOADFILENAME3]);
load(LOADFILENAME3);
disp('ext_data= ');
disp(ext_data);

%==== multiple the data matrix by 2 ==============
result = data1*data2;
disp('A * B = ');
disp(result);

%==== save  the new data to a new file ===========
SAVEPATH=strcat(pwd,filesep,'output');
if ( ~isdir(SAVEPATH))
    mkdir(SAVEPATH);
end
SAVEFILENAME=strcat(SAVEPATH,filesep,'saved_data.mat');
disp(['Save the A * B result to : ',SAVEFILENAME]);
save(SAVEFILENAME, 'result');




