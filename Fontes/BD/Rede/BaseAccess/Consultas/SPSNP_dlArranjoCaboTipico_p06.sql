  DELETE 
    cb.* 
  FROM 
    CaboZ0Z1 cb 
  WHERE 
    cb.intCaboId NOT IN (SELECT intCaboId FROM Arranjo_00_01)

