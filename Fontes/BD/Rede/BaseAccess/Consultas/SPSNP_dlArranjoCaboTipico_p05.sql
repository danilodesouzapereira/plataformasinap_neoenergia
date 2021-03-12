  DELETE 
    cb.* 
  FROM 
    CaboRGMR cb 
  WHERE 
    cb.intCaboId NOT IN (SELECT intCaboId FROM Arranjo_00_01)
