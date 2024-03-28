local started = false;

function ScriptEntry( subject, audio_obj )

    if subject == nil or audio_obj == nil then
        return 0;
    end

    if (started == true) then
        if (AudioSys.isPlaying(audio_obj) == false) then
            print("[LUA] Sound finished, returning 1");
            return 1;
        end
    end

    if (AudioSys.isPlaying(audio_obj) == false) then
        AudioSys.playSound(audio_obj, false);
        started = true;
        print("[LUA] Started sound!");
        return 0;
    end

    return 0;

end

