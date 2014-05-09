function digcam()
    h=openserial("/dev/ttyUSB0","115200,n,8,1")
    
    while(1)
        [queue,status] = serialstatus(h);
        if queue(1) > 0 then
            disp(readserial(h));
        end
    end
endfunction

