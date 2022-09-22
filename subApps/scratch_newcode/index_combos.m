% indices, ugh

%% BEST_NPZ

abbrev = 'JICO';
pm = '-+';

data = [];
grp = {};

cnt = 0;
for jelly = 0:1
    for iron = 0:1
        for carbon = 0:1
            for oxy = 0:1
                
                idx = nan(17,1);

                idx(1:12) = 1:12;
                ic = 12;
                if jelly
                    idx(13) = ic+1;
                    ic = ic+1;
                end
                if iron
                    idx(14) = ic+1;
                    ic = ic+1;
                end
                if carbon
                    idx(15:16) = ic+[1 2];
                    ic = ic+2;
                end
                if oxy
                    idx(17) = ic+1;
                    ic = ic+1;
                end
                
                cnt = cnt + 1;
                data(:,cnt) = idx;
                grp(:,cnt) = cellstr([pm([jelly iron carbon oxy]+1);abbrev]');              
            end
        end
    end
end

grp = cellfun(@(x) sprintf('%4s', x), grp, 'uni', 0);
data = arrayfun(@(x) sprintf('%4d', x), data, 'uni', 0);


writetable(cell2table([grp;data]), 'test.txt', 'delimiter', ' ');              


%% COBALT

abbrev = 'MPICD';
pm = '-+';

data = [];
grp = {};

cnt = 0;


for miner = 0:1
    for phos = 0:1
        for iron = 0:1
            for carbon = 0:1
                for diat = 0:1
                    
                    idx = nan(36,1);
                    idx(1:13) = 1:13;
                    ic = 13;
                    
                    if miner
                        idx(14:20) = ic + (1:7);
                        ic = ic + 7;
                    end
                    if phos
                        idx(21:25) = ic + (1:5);
                        ic = ic+5;
                    else
                        idx(24) = ic + 1;
                        ic = ic + 1;
                    end
                    if iron
                        idx(26:30) = ic + (1:5);
                        ic = ic+5;
                    end
                    if carbon
                        idx(31:33) = ic + (1:3);
                        ic = ic+3;
                    end
                    if diat
                        idx(34:36) = ic + (1:3);
                    end
                    
                    cnt = cnt + 1;
                    data(:,cnt) = idx;
                    grp(:,cnt) = cellstr([pm([miner phos iron carbon diat]+1);abbrev]'); 

                end
            end
        end
    end
end

grp = cellfun(@(x) sprintf('%4s', x), grp, 'uni', 0);
data = arrayfun(@(x) sprintf('%4d', x), data, 'uni', 0);

writetable(cell2table([grp;data]), 'test.txt', 'delimiter', ' ');              




