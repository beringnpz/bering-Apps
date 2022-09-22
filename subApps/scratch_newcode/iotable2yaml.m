%% I/O table to YAML

% file = '../BIO_BANAS/banas_io.csv';
file = '../BESTNPZ/bestnpz_io.csv';

Opt = detectImportOptions(file);
[Opt.VariableTypes{:}] = deal('char');
Io = readtable(file, Opt);

fld = {'index_code', 'variable', 'scale', 'io', 'long_name', 'units', 'field', 'time', 'type', 'add_offset', 'standard_name'};

Io.Properties.VariableNames = strrep(Io.Properties.VariableNames, 'variable_comment', 'io');
Io.add_offset = repmat({'0.0d0'}, height(Io),1);
Io.standard_name = repmat({''}, height(Io),1);

Io = Io(:,fld);

data = Io{:,:}';
for ii = 1:numel(data)
    if contains(data{ii}, {',',':'})
        data{ii} = sprintf('"%s",', data{ii});
    else
        data{ii} = sprintf('%s,', data{ii});
    end
end

maxlen = zeros(size(fld));
for ii = 1:length(fld)
    maxlen(ii) = max(cellfun(@length, data(ii,:)));
end
    
fmtdata = [fld; num2cell(maxlen+2)];
fmt = sprintf('%s: %%-%ds', fmtdata{:});

tmp = sprintf(['  - {' fmt '}\n'], data{:});
tmp = regexprep(tmp, ',\s*}\n', '}\n');

% newfile = strrep(file, '.csv', '.yaml');
% 
% fid = fopen(newfile, 'wt');
% fprintf(fid, 'metadata:\n%s', tmp);
% fclose(fid);

%% param table to YAML

file = '../BESTNPZ/bestnpz_parameters.csv';

Tmp = readtable(file);

fld = {'param', 'unit', 'descrip'};

data = Tmp{:,fld}';
for ii = 1:numel(data)
    if contains(data{ii}, {',',':'})
        data{ii} = sprintf('"%s",', data{ii});
    else
        data{ii} = sprintf('%s,', data{ii});
    end
end

maxlen = zeros(size(fld));
for ii = 1:length(fld)
    maxlen(ii) = max(cellfun(@length, data(ii,:)));
end
    
fmtdata = [fld; num2cell(maxlen+2)];
fmt = sprintf('%s: %%-%ds', fmtdata{:});

tmp = sprintf(['  - {' fmt '}\n'], data{:});
tmp = regexprep(tmp, ',\s*}\n', '}\n');

% newfile = strrep(file, '.csv', '.yaml');
% 
% fid = fopen(newfile, 'wt');
% fprintf(fid, 'metadata:\n%s', tmp);
% fclose(fid);

%% BIO_COBALT input to YAML

txt = fileread('../BIO_COBALT/bio_cobalt.in');

txt = regexp(txt, '\n', 'split')';
lastline = startsWith(strtrim(txt), 'TNU2');
txt = txt(1:(find(lastline)-1));

txt = strtrim(regexprep(txt, '\!.*', ''));
isemp = cellfun(@isempty, txt);
txt = txt(~isemp);

txt = txt(3:end);

pv = regexp(txt, '==', 'split');
pv = cat(1, pv{:});

val = cellfun(@str2double, pv(:,2));
isn = isnan(val);
spery = seconds(days(365.25));
val(isn) = cellfun(@eval, pv(isn,2));

tmp = [cellfun(@(x) [x ','], strtrim(pv(:,1)), 'uni', 0) num2cell(val)]';

fprintf('- {param: %-18s value: %-22.16g}\n', tmp{:});

%% ocean defaults varinfo to YAML

% file = '../ocean/varinfo.defaults.dat';
% file = '../IceBudgell/varinfo.ice.dat';
file = '../BIO_COBALT/varinfo.cobalt.dat';

newfile = strrep(file, '.dat', '.yaml');

Tmp = parsevarinfo(file);

fld = {'variable', 'long_name', 'units', 'field', 'time', 'index_code', 'type', 'scale', 'io'};
Tmp.io = Tmp.variable_comment;

data = Tmp{:,fld}';
for ii = 1:numel(data)
    if contains(data{ii}, {',',':'})
        data{ii} = sprintf('"%s",', data{ii});
    else
        data{ii} = sprintf('%s,', data{ii});
    end
end

maxlen = zeros(size(fld));
for ii = 1:length(fld)
    maxlen(ii) = max(cellfun(@length, data(ii,:)));
end
    
fmtdata = [fld; num2cell(maxlen+2)];
fmt = sprintf('%s: %%-%ds', fmtdata{:});

tmp = sprintf(['  - {' fmt '}\n'], data{:});
tmp = regexprep(tmp, ',\s*}\n', '}\n');

fid = fopen(newfile, 'wt');
fprintf(fid, 'metadata:\n%s', tmp);
fclose(fid);



