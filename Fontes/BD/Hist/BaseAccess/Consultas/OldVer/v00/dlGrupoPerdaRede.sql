DELETE FROM 
  GrupoPerdaRede 
WHERE 
  intGrupoId  = [p_GrupoId] AND
  ((intRedeId   = [p_RedeId]) OR ([p_RedeId] = -1))
  