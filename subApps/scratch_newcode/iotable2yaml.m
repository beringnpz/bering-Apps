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

newfile = strrep(file, '.csv', '.yaml');

fid = fopen(newfile, 'wt');
fprintf(fid, 'metadata:\n%s', tmp);
fclose(fid);

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

newfile = strrep(file, '.csv', '.yaml');

fid = fopen(newfile, 'wt');
fprintf(fid, 'metadata:\n%s', tmp);
fclose(fid);



