UPDATE
  (SELECT * FROM Manobra) sel
SET
  sel.strCodigo = [p_Codigo]
WHERE
  sel.intManobraId = [p_ManobraId];
