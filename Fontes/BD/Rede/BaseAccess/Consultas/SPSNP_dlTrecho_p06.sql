    DELETE 
      ArrAux.* 
    FROM 
      Arranjo_02_04 ArrAux INNER JOIN Arranjo Arr ON 
        ArrAux.intArranjoId = Arr.intArranjoId 
    WHERE 
      Arr.intArranjoId = [p_ArranjoId] AND 
      Arr.bolTipico = 0
    
