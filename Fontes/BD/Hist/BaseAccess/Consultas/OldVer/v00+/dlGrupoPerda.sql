DELETE FROM 
  GrupoPerda
WHERE 
  intGrupoId = [p_GrupoId] AND 
  intGrupoId NOT BETWEEN 1 AND 3
  