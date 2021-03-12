INSERT INTO
  Rede (
       intRedeId, strCodigo, intTipoRedeId, intRedeSupridoraId
       )
VALUES
  (
  [p_RedeId], [p_Codigo], [p_TipoRedeId], iif ( [p_SupridoraId] <> -1, [p_SupridoraId], NULL )
  )
  
  