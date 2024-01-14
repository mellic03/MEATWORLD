function GetPosition( ObjId )

end


function Vec3_add( A, B )
    return {
        x = A.x + B.x,
        y = A.y + B.y,
        z = A.z + B.z
    };
end

function Vec3_sub( A, B )
    return {
        x = A.x - B.x,
        y = A.y - B.y,
        z = A.z - B.z
    };
end

function Vec3_dot( A, B )
    return A.x*B.x + A.y*B.y + A.z*B.z;
end

function Vec3_cross( A, B )
    return {
        x = 0,
        y = 0,
        z = 0
    };
end



function MyScript( ObjId, Engine )

    Position = GetPosition(ObjId);
    Tangent  = Vec3_dot(Position, Position)
    print("Tangent: ", Tangent)

end
