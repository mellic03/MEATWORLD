function ScriptEntry( subject, audio_obj )

    if subject == nil or audio_obj == nil then
        return 0;
    end

    AudioSys.playSound(audio_obj, false);
    return 1;

end

