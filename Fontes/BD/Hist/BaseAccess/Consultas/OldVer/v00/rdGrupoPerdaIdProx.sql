SELECT 
  IIF(ISNULL(MAX(intGrupoId)), 1, MAX(intGrupoId) + 1) AS intGrupoPerdaIdProx
FROM 
  GrupoPerda
