DELETE FROM 
  PerdaTecnica
WHERE 
  dttData = [p_Data] AND
  ((intRedeId = [p_RedeId]) OR ([p_RedeId] = -1))
  