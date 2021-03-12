    DELETE 
      ArrAux.* 
    FROM 
      Arranjo_00_01 ArrAux INNER JOIN Arranjo Arr ON 
        ArrAux.intArranjoId = Arr.intArranjoId 
    WHERE 
      Arr.intArranjoId = [p_ArranjoId] AND 
      Arr.bolTipico = 0
