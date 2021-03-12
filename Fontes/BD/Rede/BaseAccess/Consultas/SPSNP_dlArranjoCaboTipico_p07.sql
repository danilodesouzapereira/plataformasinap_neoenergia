  DELETE 
    cb.* 
  FROM 
    Cabo cb 
  WHERE 
    cb.intCaboId NOT IN (SELECT intCaboId FROM Arranjo_00_01)


