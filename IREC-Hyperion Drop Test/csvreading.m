function csvreading( filename )
    close all;
    all_data = csvread(filename);

    times = all_data(:,1);

    ax_data = all_data(:,2);
    ay_data = all_data(:,3);
    az_data = all_data(:,4);

    ax_idx_max = find(ax_data == max(ax_data));
    ay_idx_max = find(ay_data == max(ay_data));
    az_idx_max = find(az_data == max(az_data));
    
    ax_max = ax_data(ax_idx_max);
    ay_max = ay_data(ay_idx_max);
    az_max = az_data(az_idx_max);
    
    ax_time_max = times(ax_idx_max);
    ay_time_max = times(ay_idx_max);
    az_time_max = times(az_idx_max);
    
    hold on;

    subplot(3,1,1);
    plot(times,ax_data);
    ax_str = ['Maximum = ',num2str(ax_max)];
    text(ax_time_max,ax_max,ax_str,'HorizontalAlignment','right');
    
    subplot(3,1,2);
    plot(times,ay_data);
    ay_str = ['Maximum = ',num2str(ay_max)];
    text(ay_time_max,ay_max,ay_str,'HorizontalAlignment','right');
    
    subplot(3,1,3);
    plot(times,az_data);
    az_str = ['Maximum = ',num2str(az_max)];
    text(az_time_max,az_max,az_str,'HorizontalAlignment','right');

    hold off;
end